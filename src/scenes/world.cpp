#include "scenes/world.hpp"

using namespace multi2d;

world_scene_t::world_scene_t(window_t&            window,
                             on_quit_fn_t         on_quit_fn,
                             bitmap_font_t&       bitmap_font)
  : scene_t(shader_t(world_vertex_code,
                     world_fragment_code), 
            window)
  , on_quit_fn_(on_quit_fn)
  , bitmap_font_(bitmap_font)
{

}

world_scene_t::~world_scene_t()
{

}

void world_scene_t::draw_scene()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  bitmap_font_.print("THIS IS A GAME", {-0.5, 0.75}, {0.5, 0.9});

  for (auto& [an, ag] : assets_) {
    for (auto& a : ag) {
      a->draw_asset();
    }
  }


  int state = glfwGetKey(window_.window(), GLFW_KEY_ESCAPE);
  if (state == GLFW_PRESS) {
    on_quit_fn_();
  }
}