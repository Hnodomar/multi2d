#ifndef PLAYER_ASSET_HPP
#define PLAYER_ASSET_HPP

#include <glm/ext.hpp>
#include <cstdint>
#include <functional>
#include <tuple>
#include <array>
#include <glm.hpp>

#include "assets/asset.hpp"
#include "assets/vertices.hpp"
#include "client/window.hpp"
#include "scenes/scene.hpp"

namespace multi2d {

  using on_player_move_fn_t = std::function<void(const uint32_t,
                                                 glm::vec2)>;

  class player_t : public asset_t, public vertices_t
  {
  public:
    player_t(const uint32_t      player_id,
             const bool          controllable,
             window_t&           window,
             on_player_move_fn_t on_player_move_fn,
             glm::vec3           pos,
             glm::vec3           scale,
             const uint32_t      shader_id);

    virtual ~player_t();

    void draw(scene_state_t& scene_state) override;

    void draw_vertices() override;

    uint32_t id();

    void update_pos(glm::vec2 pos);

  private: 

    const uint32_t id_;

    const bool controllable_;

    glm::vec3 position_;
    glm::vec3 scale_;

    glm::vec4 bl_vertex_ = {-0.25f, -0.25f, 0.0f, 1.0f};
    glm::vec4 tr_vertex_ = { 0.25f,  0.25f, 0.0f, 1.0f};

    glm::vec3 bl_cam_box_ = {-0.25f, -0.25f, 0.0f};
    glm::vec3 tr_cam_box_ = { 0.25f,  0.25f, 0.0f};

    on_player_move_fn_t on_player_move_fn_;

    window_t& window_;
  };

}

#endif