#include "assets/text_box.hpp"

using namespace multi2d;

text_box_t::text_box_t(bitmap_font_t&   bitmap_font,
                       window_t&        window,
                       const uint32_t   shader_id,
                       const glm::vec3  position,
                       glm::vec3        scale,
                       glm::vec4        colour)
  : rectangle_t(shader_id)
  , bitmap_font_(bitmap_font)
  , window_(window)
  , input_text_("                    ")
  , colour_(colour)
{
  model_ = glm::mat4(1.0f);
  model_ = glm::translate(model_, position);
  model_ = glm::scale(model_, scale);

  std::array<glm::vec4, 2> norm_dcs;
  for (auto i = 0; i < 12; i += 6) {
    glm::vec3 coord {rectangle[i],
                     rectangle[i + 1],
                     rectangle[i + 2]};
    
    glm::vec4 tsd(model_ * glm::vec4(coord, 1.0f));
    norm_dcs[i / 6] = tsd;
  }

  bl_norm_dc_ = {norm_dcs[0].x, norm_dcs[0].y};
  tr_norm_dc_ = {norm_dcs[1].x, norm_dcs[1].y};
}

std::string text_box_t::text() const
{
  return input_text_;
}

void text_box_t::draw()
{  
  set_texture();

  glUniformMatrix4fv(glGetUniformLocation(texture_id(), "model"),
                     1,
                     GL_FALSE,
                     glm::value_ptr(model_));

  glUniform4f(glGetUniformLocation(texture_id(), "button_colour"),
              colour_.x,
              colour_.y,
              colour_.z,
              1.0f);
  
  draw_vertices();

  static int idx = 0;

  for (int i = GLFW_KEY_PERIOD; i <= GLFW_KEY_SEMICOLON; ++i) {
    if (_KEY_PRESSES_[i].is_clicked_nonrepeat()) {
      if (idx != 20) { 
        if (i == GLFW_KEY_SEMICOLON) {
          i = ':';
        }
        input_text_[idx++] = i;
      }
    }
  } 

  if (_KEY_PRESSES_[GLFW_KEY_BACKSPACE].is_clicked_delay()) {
    if (!input_text_.empty() && idx >= 0) {
      input_text_[idx--] = ' ';
      idx = std::max(0, idx);
    }
  }

  bitmap_font_.print(input_text_,
                     bl_norm_dc_,
                     tr_norm_dc_,
                     glm::vec3(0.0f, 0.0f, 0.0f));
}
