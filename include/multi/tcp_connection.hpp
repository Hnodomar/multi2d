#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP

#if defined(_WIN64) || defined(_WIN32)
  #define _WIN32_WINNT 0x0600
  #include <winsock2.h>
#else
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <netinet/ip.h>
  #include <unistd.h>
#endif

#include <cstdint>
#include <functional>

#include "util/types.hpp"
#include "util/event_loop.hpp"
#include "util/exception.hpp"
#include "multi/pkt_config.hpp"
#include "multi/pkt_reader.hpp"

namespace multi2d {

  using on_read_fn_t = std::function<void (pkt_ref_t)>;

  class tcp_connection_t
  {
  public:
    tcp_connection_t(const char*    address, 
                     const uint16_t port, 
                     event_loop_t&  event_loop,  
                     on_read_fn_t   cb)
      : fd_(socket(AF_INET, SOCK_STREAM, 0))
      , sockaddr_{AF_INET, htons(port), inet_addr(address), {0}}
      , event_loop_(event_loop)
    {
      if (fd_ == -1) {
        RUNTIME_THROW(status_t::IO_ERROR,
          "Failed to open socket to '%s' on port '%i': '%s'",
          address,
          port,
          strerror(errno));
      }

      auto p = reinterpret_cast<const sockaddr*>(&sockaddr_);

      if (connect(fd_, p, sizeof(sockaddr_)) == -1) {
        RUNTIME_THROW(status_t::IO_ERROR,
          "Failed to connect to '%s' on port '%i' with socket '%i'",
          address,
          port,
          fd_);
      } 

      auto cb_fn = [&, cb](uint32_t fd) {
        
        auto pkt_ref = pkt_reader_.read(fd);

        cb(pkt_ref);
        
        return true;
      };

      event_loop_.add_event_listener(fd_, std::move(cb_fn));
    }

    tcp_connection_t(const fd_t         fd,
                     event_loop_t&      event_loop,
                     const sockaddr_in* sockaddr,
                     event_cb_t         cb)
      : fd_(fd)
      , sockaddr_(*sockaddr)
      , event_loop_(event_loop)
    {
      event_loop_.add_event_listener(fd_, std::move(cb));
    }

    ~tcp_connection_t()
    {
      event_loop_.remove_event_listener(fd_);
      close(fd_);
    }

    template<typename data_t>
    void send(pkt_t type, data_t data)
    {
      auto buff = make_pkt(type, data);

      std::cout << *(float*)(buff.data() + 3)
        << " " << *(float*)(buff.data() + 7)
        << std::endl;

      write_all(fd_, buff.data(), buff.size());
    }

  private:

    const fd_t        fd_;
    const sockaddr_in sockaddr_;
    event_loop_t&     event_loop_;
    pkt_reader_t      pkt_reader_;
  };

}

#endif
