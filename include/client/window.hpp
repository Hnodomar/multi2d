#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <cstdint>
#include <glad.h>
#include <GLFW/glfw3.h>

#include "util/exception.hpp"

namespace multi2d {

  class window_t {
  public:
    window_t(const uint16_t width, const uint16_t height)
      : width_(width)
      , height_(height)
    {
      init_window();
    }

    GLFWwindow* window()
    {
      return window_;
    }

    ~window_t()
    {
      glfwDestroyWindow(window_);
    }

    std::pair<double, double> cursor_pos() const
    {
      double x, y;
      glfwGetCursorPos(window_, &x, &y);
      return {x, y};
    }

    std::pair<uint32_t, uint32_t> window_size() const
    {
      int width, height;
      glfwGetWindowSize(window_, &width, &height);
      return {width, height};
    }

  private:

    static void fb_size_cb(GLFWwindow*   window,
                           const int32_t width,
                           const int32_t height)
    {
      glViewport(0, 0, width, height);
    }
    
    void init_window()
    {
      glfwInit();
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

      window_ = glfwCreateWindow(width_, height_, "Multi2D", nullptr, nullptr);

      glfwMakeContextCurrent(window_);

      glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      
      auto gl_ptr = reinterpret_cast<GLADloadproc>(glfwGetProcAddress);
      if (!gladLoadGLLoader(gl_ptr)) {
        RUNTIME_THROW(status_t::GLAD_FAILURE,
          "Failed to initialise GLAD");
      }

      glViewport(0, 0, width_, height_);
      glfwSetFramebufferSizeCallback(window_, fb_size_cb);
    }

    GLFWwindow*  window_;
    uint16_t     width_;
    uint16_t     height_;
  };

}

#endif