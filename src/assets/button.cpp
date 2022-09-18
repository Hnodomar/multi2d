#include "assets/button.hpp"

using namespace multi2d;

button_t::button_t(const char*        label,
                   window_t&          window,
                   const uint32_t     shader,
                   const glm::vec3    position,
                   const std::string& text,
                   bitmap_font_t&     bitmap_font,
                   const glm::vec3    scale,
                   const glm::vec4    colour)
  : label_(label)
  , shader_(shader)
  , pos_(position)
  , colour_(colour)
  , text_(text)
  , bitmap_font_(bitmap_font)
  , window_(window)
{
  model_ = glm::mat4(1.0f);
  model_ = glm::translate(model_, position);
  model_ = glm::scale(model_, scale);

  std::array<glm::vec4, 4> norm_dcs;

  for (auto i = 0; i < 12; i += 3) {
    glm::vec3 coord {rectangle[i],
                     rectangle[i + 1],
                     rectangle[i + 2]};
    
    glm::vec4 tsd(model_ * glm::vec4(coord, 1.0f));
    norm_dcs[i / 3] = tsd;
  }

  auto ws = window.window_size();
  auto window_width = ws.first;
  auto window_height = ws.second;

  auto c_b = [&](glm::vec4& v) -> glm::vec2 {
    return {
      (1.0 - ((v.x + 1.0) / 2.0)) * window_width,
      (1.0 - ((v.y + 1.0) / 2.0)) * window_height
    };
  };

  boundaries_.bottom_left = c_b(norm_dcs[0]);
  boundaries_.bottom_right = c_b(norm_dcs[1]);
  boundaries_.top_right = c_b(norm_dcs[2]);
  boundaries_.top_left = c_b(norm_dcs[3]);
}

const char* button_t::label() const
{
  return label_;
}

void button_t::draw()
{
  glUseProgram(shader_);

  glUniformMatrix4fv(glGetUniformLocation(shader_, "model"),
                     1,
                     GL_FALSE,
                     glm::value_ptr(model_));

  glUniform4f(glGetUniformLocation(shader_, "button_colour"),
              colour_.x,
              colour_.y,
              colour_.z,
              colour_.w);
  
  draw_vertices();

  if (!text_.empty()) {
    auto& b = boundaries_;

    auto [width, height] = window_.window_size();

    auto c_b = [&](const float coord, const float d) {
      return ((1.0 - (coord / d)) * 2.0) - 1.0;
    };

    bitmap_font_.print(text_,
                       c_b(b.bottom_left.x, width),
                       c_b(b.bottom_right.x, width),
                       c_b(b.bottom_left.y, height),
                       c_b(b.top_left.y, height));
  }
}

void button_t::update_boundaries_ndc(const float last_width,
                                     const float last_height,
                                     const float width,
                                     const float height)
{
  auto c_b = [&](glm::vec2& v) -> glm::vec2 {
    return {
      (v.x / last_width) * width,
      (v.y / last_height) * height
    };
  };

  boundaries_.bottom_left = c_b(boundaries_.bottom_left);
  boundaries_.bottom_right = c_b(boundaries_.bottom_right);
  boundaries_.top_left = c_b(boundaries_.top_left);
  boundaries_.top_right = c_b(boundaries_.top_right);
}

bool button_t::is_inside(const float x_pos, 
                         const float y_pos) const
{  
  auto& b = boundaries_;
  return (x_pos < b.bottom_left.x && y_pos < b.bottom_left.y) 
      && (x_pos > b.bottom_right.x && y_pos < b.bottom_right.y) 
      && (x_pos > b.top_right.x && y_pos > b.top_right.y) 
      && (x_pos < b.top_left.x && y_pos > b.top_left.y); 
}

void button_t::set_colour(glm::vec4 colour)
{
  colour_ = colour;
}
