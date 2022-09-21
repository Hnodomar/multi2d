#ifndef PKT_READER_HPP
#define PKT_READER_HPP

#include <array>
#include <cstdint>
#include <unistd.h>
#include <cstring>

#include "util/exception.hpp"
#include "util/types.hpp"
#include "multi/pkt_config.hpp"

namespace multi2d {

  using pkt_reader_buff_t = std::array<uint8_t, 256>;

  class pkt_reader_t
  {
  public:

    void read(const uint32_t fd)
    {

      switch(read_state_) {
        case read_state::READ_HEADER: {
          size_t bytes_read = ::read(fd,
                                     buffer_.data(),
                                     sizeof(pkt_header_t));
      
          if (bytes_read == -1) {
            RUNTIME_THROW(status_t::IO_ERROR,
              "Failed to read from socket %i: %s",
              fd,
              strerror(errno));
          }

          pkt_header_t& hdr = 
            *reinterpret_cast<pkt_header_t*>(buffer_.data());

          if (hdr.len > 256) {
            RUNTIME_THROW(status_t::IO_ERROR,
              "Read packet on fd %i that has length greater than 256",
              fd);
          }

        }
        break;
        case read_state::READ_PAYLOAD: {

        }
        break;
      }


    }

    pkt_reader_buff_t buffer()
    {
      return buffer_;
    }

  private:

    pkt_reader_buff_t buffer_;

    enum class read_state
    {
      READ_HEADER,
      READ_PAYLOAD
    };

    read_state read_state_ = read_state::READ_HEADER;
  };

}

#endif