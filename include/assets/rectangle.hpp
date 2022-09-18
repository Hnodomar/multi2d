#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include <glad.h>

#include "assets/asset.hpp"

namespace multi2d {

  const auto rectangle = array_of<float>(
    -0.5f, -0.15f, 0.0f,
     0.5f, -0.15f, 0.0f,
     0.5f,  0.15f, 0.0f,
    -0.5f,  0.15f, 0.0f,
     0.5f,  0.15f, 0.0f,
    -0.5f, -0.15f, 0.0f
  );

  struct rectangle_t : public asset_t
  {
    virtual ~rectangle_t() {}

    static void init_rectangle();

    static void destroy_rectangle();

    void draw_vertices() override;
  
  private:

    static uint32_t vbo_;
    static uint32_t vao_;
  };


}

#endif