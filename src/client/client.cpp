#include "client/client.hpp"

using namespace multi2d;

multi2d_client_t::multi2d_client_t(const int width, const int height)
  : window_(width, height)
  , bitmap_font_("../NANOTYPE.bff", window_)
{
  auto on_join_cb = std::bind(&multi2d_client_t::join_game,
                              this,
                              std::placeholders::_1);
  auto on_host_cb = std::bind(&multi2d_client_t::host_game, 
                              this,
                              std::placeholders::_1);

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

void multi2d_client_t::handle_client_read(pkt_ref_t pkt)
{
  if (pkt.type() > pkt_t::LAST_PKT_TYPE) {
    RUNTIME_THROW(status_t::INVALID_ARG,
      "Tried handling packet with type '%i', largest is '%i'",
      pkt.type(),
      pkt_t::LAST_PKT_TYPE);
  }

  switch (pkt.type()) {
    default:
      scene_->handle_packet(pkt);
      break;
  }
}

std::optional<tcp_server_t>& multi2d_client_t::server()
{
  return server_;
}

std::optional<tcp_connection_t>& multi2d_client_t::connection()
{
  return connection_;
}

void multi2d_client_t::join_game(const std::string& addr_and_port)
{
  auto colon_pos = addr_and_port.find(':');

  if (colon_pos == std::string::npos) {
    RUNTIME_THROW(status_t::IO_ERROR,
      "Port not supplied, must be in format 'address:port'");
  }

  const auto addr = addr_and_port.substr(0, colon_pos);
  const uint16_t port = std::stoi(addr_and_port.substr(colon_pos + 1));

  if (connection_) {
    connection_ = std::nullopt;
  }

  connection_.emplace(addr.c_str(),
                      port,
                      ev_loop_,
                      std::bind(&multi2d_client_t::handle_client_read,
                                this,
                                std::placeholders::_1));
  auto on_quit_fn = [&]() 
  {
    auto on_join_cb = [](const std::string&){};
    auto on_host_cb = std::bind(&multi2d_client_t::host_game, 
                                this,
                                std::placeholders::_1);

    set_next_scene(
      std::make_unique<menu_scene_t>(
        on_join_cb,
        on_host_cb,
        window_,
        bitmap_font_
      )
    );
  };
  
  auto world = std::make_unique<world_scene_t>(window_,
                                               on_quit_fn,
                                               bitmap_font_);

  world->add_asset(
    "players",
    std::make_unique<player_t>(
      0,
      true,
      window_,
      [&](const uint32_t id, glm::vec2 v){
        auto& c = connection();
        if (c != std::nullopt) {
          c->send(pkt_t::PLAYER_POSITON_DATA, v);
        }
        else {
          RUNTIME_THROW(status_t::IO_ERROR,
            "TCP Server does not exist when trying to send player position");
        }
      },
      glm::vec3(0.3f, 0.3f, 0.0f),
      glm::vec3(0.2f, 0.2f, 0.0f),
      world->shader_id()
    )
  );


  set_next_scene(std::move(world));
}

void multi2d_client_t::host_game(const std::string& port)
{
  auto on_quit_fn = [&]() 
  {
    auto on_join_cb = [](const std::string&){};
    auto on_host_cb = std::bind(&multi2d_client_t::host_game, 
                                this,
                                std::placeholders::_1);

    set_next_scene(
      std::make_unique<menu_scene_t>(
        on_join_cb,
        on_host_cb,
        window_,
        bitmap_font_
      )
    );
  };
  
  auto world = std::make_unique<world_scene_t>(window_,
                                               on_quit_fn,
                                               bitmap_font_);

  world->add_asset(
    "players",
    std::make_unique<player_t>(
      0,
      true,
      window_,
      [&](const uint32_t id, glm::vec2 v){
        auto& s = server();
        if (s != std::nullopt) {
          s->broadcast_data(id, pkt_t::PLAYER_POSITON_DATA, v);
        }
        else {
          RUNTIME_THROW(status_t::IO_ERROR,
            "TCP Server does not exist when trying to send player position");
        }
      },
      glm::vec3(0.3f, 0.3f, 0.0f),
      glm::vec3(0.2f, 0.2f, 0.0f),
      world->shader_id()
    )
  );

  world->enable_group("players");

  if (server_) {
    server_ = std::nullopt;
  }

  server_.emplace(ev_loop_,
                  std::bind(&multi2d_client_t::handle_client_read,
                            this,
                            std::placeholders::_1),
                  port.c_str());
  

  set_next_scene(std::move(world));
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
