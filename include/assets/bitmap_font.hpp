#ifndef BITMAP_FONT_HPP
#define BITMAP_FONT_HPP

#include <string_view>
#include <fstream>
#include <string>
#include <cstdint>
#include <cstring>
#include <array>
#include <glad.h>
#include <glm/ext.hpp>

#include "util/exception.hpp"
#include "scenes/shader.hpp"
#include "client/window.hpp"

#define BFG_RS_NONE  0x0      // Blend flags
#define BFG_RS_ALPHA 0x1
#define BFG_RS_RGB   0x2
#define BFG_RS_RGBA  0x4

namespace multi2d {

  struct bitmap_font_state_t
  {
    bitmap_font_state_t();

    ~bitmap_font_state_t()
    {

    }

    const std::string bmp_vertex_code = 
      #include "shaders/text/vertex.vs"
    ;

    const std::string bmp_fragment_code = 
      #include "shaders/text/fragment.vs"
    ;

    shader_t shader;
    uint32_t vbo;
    uint32_t vao;
    uint32_t ebo;
  };

  struct bitmap_header_t
  {
    uint16_t bitmap_id;
    
    uint32_t image_width;
    uint32_t image_height;
    uint32_t cell_width;
    uint32_t cell_height;

    uint8_t bits_per_pixel;
    uint8_t base;

    std::array<uint8_t, 256> char_widths;

  } __attribute__((__packed__));

  class bitmap_font_t
  {
  public:

    bitmap_font_t(std::string_view bitmap_filepath,
                  window_t&        window);

    uint32_t texture_id() const;

    void set_colour(const float red,
                    const float green,
                    const float blue);

    void set_screen(const float x, const float y);

    void print(const std::string& text,
               glm::vec2          bl_b,
               glm::vec2          tr_b,
               glm::vec3          colour = glm::vec3(1.0f, 1.0f, 1.0f));

    void set_print_location(const float x, const float y);

    size_t get_pixel_width(const std::string& s) const;

  private:

    void set_blend();

    void load_bitmap(std::string_view bitmap_filepath);

    bitmap_header_t header_;

    uint8_t cells_per_row_;

    float cell_width_ratio_;
    float cell_height_ratio_;

    uint32_t texture_id_;    

    uint32_t render_style_;

    float print_location_x_;
    float print_location_y_;

    uint32_t font_vao_;
    uint32_t font_vertex_buffer_id_;

    bitmap_font_state_t bmp_font_state_;

    float red_ = 1.0f, green_ = 1.0f, blue_ = 1.0f;

    bool invert_y_axis_;
    
    std::vector<uint8_t> image_;

    window_t& window_;
  };

}

#endif