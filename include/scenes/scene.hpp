#ifndef SCENE_HPP
#define SCENE_HPP

#include <glm/glm.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glad.h>
#include <array>
#include <vector>
#include <map>
#include <memory>

#include "util/exception.hpp"
#include "scenes/shader.hpp"
#include "client/window.hpp"
#include "assets/asset.hpp"
#include "multi/pkt_config.hpp"

namespace multi2d {

  class asset_t;

  struct scene_state_t
  {
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec2 cam_vec = {0.0f, 0.0f};
  };

  class scene_t
  {
  public:

    scene_t(const shader_t shader, window_t& window);

    virtual ~scene_t();

    virtual void draw_scene() = 0;

    virtual void handle_packet(pkt_ref_t pkt) = 0;

    void add_asset(const char* group, std::unique_ptr<asset_t> asset);

    uint32_t shader_id() const;
  
    void enable_group(const char* group);

    scene_state_t& state();

  protected:

    shader_t              shader_;
    window_t&             window_;
    scene_state_t         state_;

    using asset_group_name_t = const char*;
    using asset_group_t = std::vector<std::unique_ptr<asset_t>>;

    std::map<asset_group_name_t, asset_group_t> assets_;
  };

}

#endif