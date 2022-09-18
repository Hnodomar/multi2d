#ifndef BG_IMAGE_HPP
#define BG_IMAGE_HPP

#include <string_view>
#include <cstdint>

#include <glad.h>

#include "util/exception.hpp"
#include "scenes/shader.hpp"
#include "client/window.hpp"
#include "assets/bitmap_font.hpp"

namespace multi2d {

  struct bg_image_vertex_buffer_t
  {
    bg_image_vertex_buffer_t();
  
    ~bg_image_vertex_buffer_t();

    static constexpr std::array<float, 32> vertex_buffer_data_ = {
      // positions          // colors       // texture coords
      1.0f,  1.0f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // top right
      1.0f, -1.0f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  // bottom right
      -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // bottom left
      -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f   // top left
    };

    static constexpr std::array<uint32_t, 6> indices_ = {
      0, 1, 3,
      1, 2, 3
    };

    const std::string bg_vertex_code = 
      #include "shaders/background/vertex.vs"
    ;

    const std::string bg_fragment_code = 
      #include "shaders/background/fragment.vs"
    ;

    shader_t shader_;

    uint32_t vbo_;
    uint32_t vao_;
    uint32_t ebo_;
  };

  class bg_image_t
  {
  public:

    bg_image_t(std::string_view filename, window_t& w);

    void draw(const uint32_t x, const uint32_t y);

  private:

    void ortho_start(const uint32_t x, const uint32_t y);

    void ortho_end();
      
    uint32_t texture_id_;
    uint32_t texture_width_;
    uint32_t texture_height_;

    window_t& window_;

    bg_image_vertex_buffer_t vbuffer_state_;
  };
}


#endif