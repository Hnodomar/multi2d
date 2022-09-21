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

  glfwSetKeyCallback(window_.window(), key_cb);
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
      on_quit_fn,
      bitmap_font_
    )
  );

  next_scene_->add_asset(
    "players",
    std::make_unique<player_t>(
      0,
      true,
      window_,
      [&](const uint32_t id, const int32_t x, const int32_t y){
        if (server_) {
          server_->broadcast_data(id, std::make_pair(x, y));
        }
        else {
          RUNTIME_THROW(status_t::IO_ERROR,
            "TCP Server does not exist when trying to send player position");
        }
      },
      glm::vec3(0.3f, 0.3f, 0.0f),
      glm::vec3(0.2f, 0.2f, 0.0f),
      next_scene_->shader_id()
    )
  );

  next_scene_->enable_group("players");

  if (!server_) {
    auto on_connect = [&, world=next_scene_.get()](const uint32_t fd){
      world->add_asset(
        "players",
        std::make_unique<player_t>(
          fd,
          false,
          window_,
          [](const uint32_t id, const int32_t x, const int32_t y){

          },
          glm::vec3(0.3f, 0.3f, 0.0f),
          glm::vec3(0.2f, 0.2f, 0.0f),
          world->shader_id()
        )
      );
    };

    server_.emplace(ev_loop_,
                    on_connect,
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
