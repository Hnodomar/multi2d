#ifndef WORLD_HPP
#define WORLD_HPP

#include <iostream>
#include <memory>
#include <glad.h>
#include <functional>

#include "scenes/scene.hpp"
#include "client/window.hpp"
#include "assets/asset.hpp"
#include "assets/bitmap_font.hpp"

namespace multi2d {

  using on_quit_fn_t = std::function<void()>;

  const std::string world_vertex_code = 
  #include "shaders/world/vertex.vs"
  ;

  const std::string world_fragment_code = 
  #include "shaders/world/fragment.vs"
  ;


  class world_scene_t : public scene_t
  {
  public:

    world_scene_t(window_t& window,
                  on_quit_fn_t on_quit_fn,
                  bitmap_font_t& bitmap_font);

    virtual ~world_scene_t();

    void draw_scene() override;

  private:

    on_quit_fn_t   on_quit_fn_;

    bitmap_font_t& bitmap_font_;

  };

}

#endif