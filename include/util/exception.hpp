#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <cstdio>
#include <array>

#include "util/types.hpp"
#include "stdarg.h"

namespace multi2d {

  struct error_t
  {
    static error_t make(status_t    status,
                        const char* fmt,
                        ...)
    {
      va_list vargs;
      va_start(vargs, fmt);    

      error_t error;

      std::vsnprintf(error.error_msg.data(),
                     error.error_msg.size() - 1,
                     fmt,
                     vargs);

      va_end(vargs);

      return error;
    }

    std::array<char, 256> error_msg;
  }; 

  class exception_t : public std::exception
  {
  public:
    exception_t(const error_t& error)
      : error_(error)
    {}

    error_t const& error() const
    {
      return error_;
    }
    
    const char* what() const noexcept override
    {
      return error_.error_msg.data();
    }
  private:
    error_t error_;
  };

  #define RUNTIME_THROW(STATUS, ...) \
    throw exception_t(error_t::make(STATUS, __VA_ARGS__))

}

#endif