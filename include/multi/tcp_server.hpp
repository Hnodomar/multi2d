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
#include "multi/tcp_connection.hpp"

namespace multi2d {

  using on_connect_fn_t = std::function<void (const uint32_t)>;
  using on_client_read_fn_t = std::function<bool()>;
  using on_client_write_fn_t = std::function<void(const fd_t)>;

  class tcp_server_t
  {
  public:
    tcp_server_t(event_loop_t&       el,
                 on_connect_fn_t     on_connect_fn,
                 on_client_read_fn_t on_read_fn,
                 addrinfo* a = default_addrinfo(),
                 const int backlog = 10) 
      : server_fd_(socket(a->ai_family, a->ai_socktype, a->ai_protocol))
      , server_sockaddr_(*a->ai_addr)
      , el_(el)
    { 
      if (bind(server_fd_, a->ai_addr, a->ai_addrlen) == -1) {
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

      auto on_connect = [&, on_read_fn, on_connect_fn](const int32_t fd) {
        
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
          
          


          return true;
        };

        on_connect_fn(user_fd);

        broadcast_data(user_fd, std::make_pair(0.3f, 0.3f));

        client_sessions_.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(server_fd_),
                                 std::forward_as_tuple(user_fd,
                                                       reinterpret_cast<sockaddr_in*>(&addr),
                                                       el_,
                                                       on_client_read_fn));

        return true;
      };

      el.add_event_listener(server_fd_, std::move(on_connect));
    }

    template<typename data_t>
    void broadcast_data(const fd_t origin, data_t data)
    {
      for (auto& [fd, conn] : client_sessions_) {
        if (fd != origin) {
          //conn.send(); // broadcast position to all clients
        }
      }
    }

  private:

    static addrinfo* default_addrinfo()
    {
      addrinfo hints, *resp;

      std::memset(&hints, 0, sizeof(hints));
      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_flags = AI_PASSIVE;

      const auto ret = getaddrinfo(NULL, "9100", &hints, &resp);

      if (ret != 0) {
        RUNTIME_THROW(status_t::IO_ERROR,
          "TCP Server failed to setup default addrinfo: '%s'",
          gai_strerror(ret));
      }

      return resp;
    }

    const fd_t                                 server_fd_;
    const sockaddr                             server_sockaddr_;
    std::unordered_map<fd_t, tcp_connection_t> client_sessions_;
    event_loop_t&                              el_;
  };

}

#endif