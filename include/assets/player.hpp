#ifndef PLAYER_ASSET_HPP
#define PLAYER_ASSET_HPP

#include <glm/ext.hpp>
#include <cstdint>
#include <functional>
#include <tuple>
#include <array>

#include "assets/asset.hpp"
#include "assets/vertices.hpp"
#include "client/window.hpp"

namespace multi2d {

  using on_player_move_fn_t = std::function<void(const uint32_t,
                                                 const int32_t, 
                                                 const int32_t)>;

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

    void draw() override;

    void draw_vertices() override;

  private: 

    const uint32_t id_;

    const bool controllable_;

    glm::mat4 model_;
    glm::vec3 position_;
    glm::vec3 scale_;

    on_player_move_fn_t on_player_move_fn_;

    window_t& window_;
  };

}

#endif