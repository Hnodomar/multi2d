#include "assets/rectangle.hpp"

using namespace multi2d;

uint32_t rectangle_t::vbo_ = 0;
uint32_t rectangle_t::vao_ = 0;

void rectangle_t::init_rectangle()
{
  if (vao_ != 0 || vbo_ != 0) {
    return;
  }

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

void rectangle_t::destroy_rectangle()
{
  if (vao_ == 0 || vbo_ == 0) {
    return;
  }
  
  glDeleteVertexArrays(1, &vao_);
  glDeleteBuffers(1, &vbo_);
}

void rectangle_t::draw_vertices()
{
  glBindVertexArray(vao_);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}
