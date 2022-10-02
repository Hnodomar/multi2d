#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <cstdint>
#include <iostream>
#include <array>
#include <cstring>
#include <unistd.h>

#include "util/exception.hpp"
#include "multi/pkt_config.hpp"

namespace multi2d {

#if defined(_WIN64) || defined(_WIN32)
  
  #include <winsock2.h>

  char* windows_error_str(const int);
#else
  #include <sys/socket.h>
#endif  

  void write_all(int32_t fd, uint8_t* data, uint16_t length);
}

#endif