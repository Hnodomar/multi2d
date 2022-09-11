#ifndef EVENT_LOOP_HPP
#define EVENT_LOOP_HPP

#include <unordered_map>
#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

#if defined(_WIN64) || defined(_WIN32)
  #define _WIN32_WINNT 0x0600
  #define POLL_FN WSAPoll
  #define STR_ERROR_FN windows_error_str
  #include <winsock2.h>
#else
  #include <poll.h>
  #define STR_ERROR_FN strerror
  #define POLL_FN(x, y, z)::poll(x, y, z)
#endif

#include "util/util.hpp"
#include "util/types.hpp"
#include "util/exception.hpp"

namespace multi2d {

  class event_loop_t
  {
  public:

    void add_event_listener(const fd_t fd, const event_cb_t cb);

    void remove_event_listener(const fd_t fd);

    void poll();

  private:

    std::vector<pollfd> poll_fds_;

    std::unordered_map<fd_t, event_cb_t> event_callbacks_;
  };

}

#endif