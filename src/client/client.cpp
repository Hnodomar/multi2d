#include "client/client.hpp"

using namespace multi2d;

multi2d_client_t::multi2d_client_t(const int width, const int height)
  : window_(width, height)
  , bitmap_font_("../NANOTYPE.bff", window_)
{
  auto on_join_cb = [](){};
  auto on_host_cb = std::bind(&multi2d_client_t::host_game, 
                              this);

  auto menu = std::move(
                std::make_unique<menu_scene_t>(on_join_cb,
                                               on_host_cb,
                                               window_,
                                               bitmap_font_));
  set_scene(std::move(menu));
}

void multi2d_client_t::set_next_scene(std::unique_ptr<scene_t> scene)
{
  next_scene_ = std::move(scene);
}

void multi2d_client_t::host_game()
{
  auto on_quit_fn = [&]() 
  {
    auto on_join_cb = [](){};
    auto on_host_cb = std::bind(&multi2d_client_t::host_game, 
                                this);

    set_next_scene(
      std::make_unique<menu_scene_t>(
        on_join_cb,
        on_host_cb,
        window_,
        bitmap_font_
      )
    );
  };

  set_next_scene(
    std::make_unique<world_scene_t>(
      window_,
      on_quit_fn
    )
  );

  if (!server_) {
    server_.emplace(ev_loop_,
                    [&]() { 
                      
                      
                      return true;
                    });
  }
}

void multi2d_client_t::start()
{
  render_loop();
}

void multi2d_client_t::render_loop()
{
  while (!glfwWindowShouldClose(window_.window())) {
    glfwPollEvents();
    scene_->draw_scene();

    if (next_scene_.get()) {
      scene_ = std::move(next_scene_);
      next_scene_ = nullptr;
    }

    glfwSwapBuffers(window_.window());
    ev_loop_.poll();
  }
}

void multi2d_client_t::set_scene(std::unique_ptr<scene_t> scene)
{
  scene_ = std::move(scene);
}
