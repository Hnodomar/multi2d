#ifndef TYPES_HPP
#define TYPES_HPP

#include <functional>
#include <cstdint>

namespace multi2d {
  using fd_t = int32_t;
  using event_cb_t = std::function<bool(const uint32_t)>;

  enum class status_t
  {
    IO_ERROR,
    OVERFLOW,
    UNDERFLOW,
    INVALID_ARG,
    UNEXPECTED_BEHAVIOUR,
    TASK_FAILURE,
    GLAD_FAILURE,
    OPENGL_FAILURE
  };
  
  
}

#endif