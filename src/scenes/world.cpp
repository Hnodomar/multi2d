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

  bitmap_font_.print("THIS IS A GAME", 
                     {-0.5, 0.75},
                     {0.5, 0.9},
                     glm::vec3(1.0f, 1.0f, 1.0f),
                     glm::translate(glm::mat4(1.0f),
                                    {state_.cam_vec.x,
                                     state_.cam_vec.y,
                                     0.0f}));

  for (auto& [an, ag] : assets_) {
    for (auto& a : ag) {
      a->draw_asset(state_);
    }
  }

  int state = glfwGetKey(window_.window(), GLFW_KEY_ESCAPE);
  if (state == GLFW_PRESS) {
    on_quit_fn_();
  }
}

void world_scene_t::update_player_pos(int32_t id, glm::vec2 pos)
{
  auto it = id_to_player_.find(id);
  if (it == id_to_player_.end()) {
    auto& players = assets_["players"];
    auto player = std::make_unique<player_t>(id, 
                                             false,
                                             window_,
                                             [](const uint32_t,
                                                glm::vec2){},
                                             glm::vec3(0.0f, 0.0f, 0.0f),
                                             glm::vec3(0.2f, 0.2f, 0.0f),
                                             shader_.get_id());
    player->enable();
    id_to_player_.emplace(id, *player.get());
    players.emplace_back(std::move(player));
  }
  else {
    it->second.get().update_pos(pos);
  }
}

void world_scene_t::set_camera_vec(glm::vec3 v)
{
  camera_vec_ = v;
}

void world_scene_t::handle_packet(pkt_ref_t pkt)
{
  switch (pkt.type()) {
    case pkt_t::PLAYER_POSITON_DATA:
      update_player_pos(pkt.id(), 
                        *reinterpret_cast<glm::vec2*>(pkt.payload()));
      break;
    default:
      RUNTIME_THROW(status_t::UNEXPECTED_BEHAVIOUR,
        "No handling case for packet type '%i'",
        pkt.type());
  }
}
