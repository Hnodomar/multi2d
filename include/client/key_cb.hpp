#ifndef KEY_CB_HPP
#define KEY_CB_HPP

#include <array>
#include <map>
#include <iostream>

#include <GLFW/glfw3.h>

#include "util/exception.hpp"


namespace multi2d {

struct key_metadata_t
{
  bool is_clicked_delay() 
  {
    float now = glfwGetTime();
    float interval = now - last_time;

    const bool clicked = (curr_state == GLFW_PRESS
                          || curr_state == GLFW_REPEAT);
    if (clicked && interval > 0.15f) {

      last_time = now;

      return true;
    }

    return false;
  }

  bool is_clicked_nonrepeat()
  { 
    const bool ret = (curr_state == GLFW_RELEASE
                      && last_state == GLFW_PRESS
                      && !used);

    used = true;

    return ret;
  }

  void update(int last, int curr)
  {
    last_state = last;
    curr_state = curr;

    used = false; 
  }

  float last_time = glfwGetTime();
  bool used = false;
  int last_state = GLFW_RELEASE;
  int curr_state = GLFW_RELEASE;
};

extern key_metadata_t _KEY_PRESSES_[GLFW_KEY_LAST];


static inline void key_cb(GLFWwindow* w, int key, int sc, int action, int mods)
{
  if (key == GLFW_KEY_UNKNOWN) {
    RUNTIME_THROW(status_t::IO_ERROR,
      "Key callback called with unknown key");
  }

  _KEY_PRESSES_[key].update(_KEY_PRESSES_[key].curr_state,
                            action);
}

}

#endif