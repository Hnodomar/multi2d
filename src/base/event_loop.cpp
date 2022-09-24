#include "util/event_loop.hpp"

using namespace multi2d;

void event_loop_t::add_event_listener(const fd_t fd, const event_cb_t cb)
{
  event_callbacks_[fd] = std::move(cb);

  poll_fds_.push_back({fd, POLLIN, NULL});
}

void event_loop_t::remove_event_listener(const fd_t fd)
{
  if (event_callbacks_.empty() || poll_fds_.empty()) {
    RUNTIME_THROW(status_t::INVALID_ARG,
      "Tried removing event listener with fd '%i' when there aren't any",
      fd);
  }

  auto it = event_callbacks_.find(fd);

  if (it == event_callbacks_.end()) {
    RUNTIME_THROW(status_t::INVALID_ARG,
      "Tried removing event listener with fd '%i' that doesn't correspond to any",
      fd);
  }

  event_callbacks_.erase(it);

  auto poll_fd_it = std::find_if(
    poll_fds_.begin(),
    poll_fds_.end(),
    [fd](const auto& poll_fd){return fd == poll_fd.fd;}
  );

  if (poll_fd_it == poll_fds_.end()) {
    RUNTIME_THROW(status_t::INVALID_ARG,
      "Tried removing event listener with fd '%i' that corresponds to a event cb but not a poll_fd",
      fd);
  }

  auto last = poll_fds_.end() - 1;

  auto& empty_place = *poll_fd_it;
  empty_place = *last;

  poll_fds_.erase(last);
}

void event_loop_t::poll()
{
  if (!poll_fds_.empty()) {
    const auto poll_ret = POLL_FN(poll_fds_.data(), 
                                  poll_fds_.size(), 
                                  0);
    
    if (poll_ret == -1) {
      RUNTIME_THROW(status_t::IO_ERROR,
        "Polling failed: '%s'", STR_ERROR_FN(errno));
    }

    for (const auto& poll_fd : poll_fds_) {
      auto& event_cb = event_callbacks_[poll_fd.fd];

      const bool readable = poll_fd.revents & POLLIN;

      if (readable && !event_cb(poll_fd.fd)) {
        RUNTIME_THROW(status_t::TASK_FAILURE,
          "Event callback task failed for fd '%i'",
          poll_fd.fd);
      }
    }
  }
}
