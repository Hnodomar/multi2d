#ifndef SCENE_HPP
#define SCENE_HPP

#include <glm/glm.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glad.h>
#include <array>
#include <vector>

#include "util/exception.hpp"
#include "scenes/shader.hpp"
#include "client/window.hpp"

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
      for(const auto& vao : vaos_) {
        glDeleteVertexArrays(1, &vao);
      }

      for (const auto& vbo : vbos_) {
        glDeleteBuffers(1, &vbo);
      }
    }

    void add_vao(const uint32_t vao)
    {
      vaos_.push_back(vao);
    }

    void add_vbo(const uint32_t vbo)
    {
      vbos_.push_back(vbo);
    }

    std::vector<uint32_t>& vaos()
    {
      return vaos_;
    }

    virtual void draw_scene() = 0;

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
    std::vector<uint32_t> vaos_;
    std::vector<uint32_t> vbos_;
    shader_t              shader_;
    window_t&             window_;
  };

}

#endif