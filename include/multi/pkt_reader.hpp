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

    pkt_ref_t read(const uint32_t fd)
    {
      int bytes_read = ::read(fd,
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

      if (hdr.length > 256) {
        RUNTIME_THROW(status_t::IO_ERROR,
          "Read packet on fd %i that has length greater than 256",
          fd);
      }
    
      uint8_t* pld_ptr = buffer_.data() + sizeof(pkt_header_t);
      bytes_read = ::read(fd,
                          pld_ptr,
                          hdr.length);
      
      if (bytes_read == -1) {
        RUNTIME_THROW(status_t::IO_ERROR,
          "Failed to read from socket %i: %s",
          fd,
          strerror(errno));
      }
      
     std::cout << (int)static_cast<pkt_t>(*buffer_.data()) << " "
        << *(uint16_t*)(buffer_.data() + 1) << " "
        << *(float*)(buffer_.data() + 3)
        << " " << *(float*)(buffer_.data() + 7)
        << std::endl;

      return pkt_ref_t(buffer_.data(), fd);
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
  };
}

#endif
