#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <cstdint>
#include <array>
#include <cstring>
#include <unistd.h>

#include "util/exception.hpp"
#include "multi/pkt_config.hpp"

namespace multi2d {

#if defined(_WIN64) || defined(_WIN32)
  
  #include <winsock2.h>

  std::string windows_error_str(const int);
#endif  

  void write_all(int32_t fd, uint8_t* data, uint16_t length);

  template<typename data_t>
  auto make_pkt(pkt_t type, data_t data)
    -> std::array<uint8_t, sizeof(multi2d::pkt_header_t) + sizeof(data_t)>
  {
    constexpr auto pkt_size = sizeof(pkt_header_t) + sizeof(data_t);
    std::array<uint8_t, pkt_size> buff;
  
    buff[0] = static_cast<uint8_t>(type);

    const uint16_t len = sizeof(data_t);
    std::memcpy(buff.data() + 1, &len, sizeof(len));

    uint8_t* pld_ptr = buff.data() + sizeof(pkt_header_t);
    std::memcpy(pld_ptr, &data, len);

    return buff;
  }
}

#endif