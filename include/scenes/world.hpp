#ifndef WORLD_HPP
#define WORLD_HPP

#include <iostream>
#include <glad.h>
#include <functional>

#include "scenes/scene.hpp"
#include "client/window.hpp"

namespace multi2d {

  using on_quit_fn_t = std::function<void()>;

  const std::string world_vertex_code = 
  #include "shaders/menu/vertex.vs"
  ;

  const std::string world_fragment_code = 
  #include "shaders/menu/fragment.vs"
  ;


  class world_scene_t : public scene_t
  {
  public:

    world_scene_t(window_t& window,
                  on_quit_fn_t on_quit_fn);

    virtual ~world_scene_t();

    void draw_scene() override;

  private:

    on_quit_fn_t on_quit_fn_;

  };

}

#endif