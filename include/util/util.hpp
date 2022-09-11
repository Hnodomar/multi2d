#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>

namespace multi2d {

#if defined(_WIN64) || defined(_WIN32)
  
  #include <winsock2.h>

  std::string windows_error_str(const int);
#endif

}

#endif