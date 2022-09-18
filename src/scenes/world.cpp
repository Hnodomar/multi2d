#include "scenes/world.hpp"

using namespace multi2d;

world_scene_t::world_scene_t(window_t&    window,
                             on_quit_fn_t on_quit_fn)
  : scene_t(shader_t(world_vertex_code,
                     world_fragment_code), 
            window)
  , on_quit_fn_(on_quit_fn)
{

}

world_scene_t::~world_scene_t()
{

}

void world_scene_t::draw_scene()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  int state = glfwGetKey(window_.window(), GLFW_KEY_ESCAPE);
  if (state == GLFW_PRESS) {
    on_quit_fn_();
  }
}