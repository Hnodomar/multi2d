#include "client/client.hpp"

using namespace multi2d;

multi2d_client_t::multi2d_client_t(const int width, const int height)
  : window_(width, height)
  , bitmap_font_("../bitmap_font.bff")
{
  auto on_join_cb = [](){};
  auto on_host_cb = [](){};

  auto menu = std::move(
                std::make_unique<menu_scene_t>(on_join_cb,
                                               on_host_cb,
                                               window_,
                                               bitmap_font_));

  set_scene(std::move(menu));
}

void multi2d_client_t::start()
{
  render_loop();
}

void multi2d_client_t::render_loop()
{
  while (!glfwWindowShouldClose(window_.window())) {
    glfwSwapBuffers(window_.window());
    glfwPollEvents();
    scene_->draw_scene();
    scene_->process_input();
    ev_loop_.poll();
  }
}

void multi2d_client_t::set_scene(std::unique_ptr<scene_t> scene)
{
  scene_ = std::move(scene);
}