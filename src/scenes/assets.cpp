#include "scenes/assets.hpp"

using namespace multi2d::assets;

uint32_t geometry::rectangle_t::vbo_ = 0;
uint32_t geometry::rectangle_t::vao_ = 0;


void geometry::rectangle_t::init_rectangle()
{
  glGenVertexArrays(1, &vao_);

  glGenBuffers(1, &vbo_);

  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  
  glBufferData(GL_ARRAY_BUFFER,
               rectangle.size() * sizeof(float),
               rectangle.data(),
               GL_STATIC_DRAW);
  
  glVertexAttribPointer(0,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        3 * sizeof(float),
                        nullptr);

  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
}

void geometry::rectangle_t::destroy_rectangle()
{
  glDeleteVertexArrays(1, &vao_);
  glDeleteBuffers(1, &vbo_);
}

void geometry::rectangle_t::draw_vertices()
{
  glBindVertexArray(vao_);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

menus::button_t::button_t(const char*        label,
                          const uint32_t     window_width,
                          const uint32_t     window_height,
                          const uint32_t     shader,
                          const glm::vec3    position,
                          const std::string& text,
                          const glm::vec3    scale,
                          const glm::vec4    colour)
  : label_(label)
  , shader_(shader)
  , pos_(position)
  , colour_(colour)
  , text_(text)
{
  model_ = glm::mat4(1.0f);
  model_ = glm::translate(model_, position);
  model_ = glm::scale(model_, scale);

  std::array<glm::vec4, 4> norm_dcs;

  for (auto i = 0; i < 12; i += 3) {
    glm::vec3 coord {geometry::rectangle[i],
                     geometry::rectangle[i + 1],
                     geometry::rectangle[i + 2]};
    
    glm::vec4 tsd(model_ * glm::vec4(coord, 1.0f));
    norm_dcs[i / 3] = tsd;
  }

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

const char* menus::button_t::label() const
{
  return label_;
}

void menus::button_t::draw()
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
}

void menus::button_t::update_boundaries_ndc(const float last_width,
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

bool menus::button_t::is_inside(const float x_pos, 
                                const float y_pos) const
{  
  auto& b = boundaries_;
  return (x_pos < b.bottom_left.x && y_pos < b.bottom_left.y) 
      && (x_pos > b.bottom_right.x && y_pos < b.bottom_right.y) 
      && (x_pos > b.top_right.x && y_pos > b.top_right.y) 
      && (x_pos < b.top_left.x && y_pos > b.top_left.y); 
}

void menus::button_t::set_colour(glm::vec4 colour)
{
  colour_ = colour;
}
