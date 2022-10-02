#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#if defined(_WIN64) || defined(_WIN32)
  #define _WIN32_WINNT 0x0600
  #include <winsock2.h>
  #include <ws2tcpip.h>
#else
  #include <sys/socket.h>
  #include <sys/types.h>
  #include <netdb.h>
  #include <netinet/ip.h>
#endif

#include <unordered_map>

#include "assets/asset.hpp"
#include "util/types.hpp"
#include "util/event_loop.hpp"
#include "util/util.hpp"
#include "multi/tcp_connection.hpp"
#include "multi/pkt_reader.hpp"
#include "multi/pkt_config.hpp"

namespace multi2d {

  using on_connect_fn_t = std::function<void (const uint32_t)>;
  using on_client_read_fn_t = std::function<void (pkt_ref_t)>;
  using on_client_write_fn_t = std::function<void (const fd_t)>;

  class tcp_server_t
  {
  public:
    tcp_server_t(event_loop_t&       el,
                 on_client_read_fn_t on_read_fn,
                 const char*         port,
                 const int           backlog = 10) 
      : el_(el)
    { 
      auto s = default_addrinfo(port);

      server_fd_ = socket(s.ai_family, s.ai_socktype, s.ai_protocol);
      if (server_fd_ == -1) {
        RUNTIME_THROW(status_t::IO_ERROR,
          "TCP Server failed to set up socket: '%s'",
          strerror(errno));
      }

      sockaddr_in addr;
      try {
        addr.sin_port = htons(std::stoi(port));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");
      }
      catch (const std::exception& e) {
        RUNTIME_THROW(status_t::UNEXPECTED_BEHAVIOUR,
          "Failed conversion of port str to uint16_t: %s",
          e.what());
      }

      sockaddr* addr_ptr = reinterpret_cast<sockaddr*>(&addr);
      if (bind(server_fd_, s.ai_addr, s.ai_addrlen) == -1) {
        RUNTIME_THROW(status_t::IO_ERROR,
          "TCP Server failed to bind on fd '%i': '%s'",
          server_fd_,
          strerror(errno));
      }

      if (listen(server_fd_, backlog) == -1) {
        RUNTIME_THROW(status_t::IO_ERROR,
          "TCP Server failed to listen on fd '%i': '%s'",
          server_fd_,
          strerror(errno));
      }

      auto on_connect = [&, on_read_fn](const int32_t fd) {
        
        sockaddr addr;
        socklen_t addr_size = sizeof(addr);

        const int user_fd = accept(server_fd_, &addr, &addr_size);

        if (user_fd == -1) {
          RUNTIME_THROW(status_t::IO_ERROR,
            "TCP Server failed to accept on fd '%i': '%s'",
            server_fd_,
            strerror(errno));
          
          return false;
        }

        auto on_client_read_fn = [&, on_read_fn](const uint32_t fd){
      
          auto pkt_ref = pkt_reader_.read(fd);

          on_read_fn(pkt_ref);

          return true;
        };

        client_sessions_.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(user_fd),
                                 std::forward_as_tuple(user_fd,
                                                       el_,
                                                       reinterpret_cast<sockaddr_in*>(&addr),
                                                       on_client_read_fn));
        return true;
      };

      el.add_event_listener(server_fd_, std::move(on_connect));
    }
    
    template<typename data_t>
    void broadcast_data(const fd_t origin, pkt_t type, data_t data)
    {
      auto buff = pkt_ref_t::make_pkt(type, data);

      for (auto& [fd, conn] : client_sessions_) {
        if (fd != origin) {
          write_all(fd, buff.data(), buff.size());
        }
      }
    }

  private:

    static addrinfo default_addrinfo(const char* port)
    {
      addrinfo hints, *resp;

      std::memset(&hints, 0, sizeof(hints));
      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_flags = AI_PASSIVE;

      const auto ret = getaddrinfo(NULL, "9000", &hints, &resp);

      if (ret != 0) {
        RUNTIME_THROW(status_t::IO_ERROR,
          "TCP Server failed to setup default addrinfo: '%s'",
          gai_strerror(ret));
      }

      return *resp;
    }

    fd_t                                       server_fd_;
    sockaddr                                   server_sockaddr_;
    std::unordered_map<fd_t, tcp_connection_t> client_sessions_;
    event_loop_t&                              el_;
    pkt_reader_t                               pkt_reader_; 
  };

}

#endif
