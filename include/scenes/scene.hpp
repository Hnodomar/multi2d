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

namespace multi2d {

  class scene_t
  {
  public:

    scene_t(const shader_t shader, window_t& window)
      : shader_(shader)
      , window_(window)
    {

    }

    virtual ~scene_t() 
    {
    }

    virtual void draw_scene() = 0;

    void add_asset(const char* group, std::unique_ptr<asset_t> asset)
    {
      asset->enable();
      assets_[group].emplace_back(std::move(asset));
    }

    uint32_t shader_id() const
    {
      return shader_.get_id();
    }
  
    void enable_group(const char* group)
    {
      auto it = assets_.find(group);

      if (it == assets_.end()) {
        RUNTIME_THROW(status_t::INVALID_ARG,
          "Tried enabling asset group %s which does not exist",
          group);
      }

      for (auto& asset : it->second) {
        asset->enable();
      }
    }

  protected:

    glm::vec3 window_to_obj_coords(const glm::vec3& w_coord)
    {
      glm::mat4 model_matrix, projection_matrix;

      glm::highp_vec4 viewport;

      glGetFloatv(GL_MODELVIEW_MATRIX, 
        reinterpret_cast<GLfloat*>(&model_matrix));
      glGetFloatv(GL_PROJECTION_MATRIX, 
        reinterpret_cast<GLfloat*>(&projection_matrix));

      glGetFloatv(GL_VIEWPORT, &viewport.x);

      return glm::unProject(w_coord, 
                            model_matrix,
                            projection_matrix,
                            viewport);
    }
    
    glm::mat4             model_;
    shader_t              shader_;
    window_t&             window_;

    using asset_group_name_t = const char*;
    using asset_group_t = std::vector<std::unique_ptr<asset_t>>;

    std::map<asset_group_name_t, asset_group_t> assets_;
  };

}

#endif