#include "assets/player.hpp"

#include <iostream>

using namespace multi2d;

player_t::player_t(const uint32_t      player_id,
                   const bool          controllable,
                   window_t&           window,
                   on_player_move_fn_t on_player_move_fn,
                   glm::vec3           pos,
                   glm::vec3           scale,
                   const uint32_t      shader_id)
  : asset_t(shader_id)
  , vertices_t([]()
    {
      uint32_t vao_, vbo_, ebo_;
      glGenVertexArrays(1, &vao_);
      glGenBuffers(1, &vbo_);
      glGenBuffers(1, &ebo_);

      constexpr std::array<float, 12> vertex_buffer_data = {
        0.25f,  0.25f, 0.0f,
        0.25f, -0.25f, 0.0f,
        -0.25f, -0.25f, 0.0f,
        -0.25f,  0.25f, 0.0f
      };

      constexpr std::array<uint32_t, 6> indices = {
        0, 1, 3,
        1, 2, 3
      };

      glBindVertexArray(vao_);
      glBindBuffer(GL_ARRAY_BUFFER, vbo_);
      glBufferData(GL_ARRAY_BUFFER,
                  vertex_buffer_data.size() * sizeof(float),
                  vertex_buffer_data.data(),
                  GL_STATIC_DRAW);

      constexpr auto stride = 3 * sizeof(float);
      
      glVertexAttribPointer(0,
                            3,
                            GL_FLOAT,
                            GL_FALSE,
                            stride,
                            (void*)0);
      glEnableVertexAttribArray(0);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                  indices.size() * sizeof(uint32_t),
                  indices.data(),
                  GL_STATIC_DRAW);

      glBindVertexArray(0);

      return std::tuple{vbo_, ebo_, vao_};
    })
  , id_(player_id)
  , controllable_(controllable)
  , position_(pos)
  , scale_(scale)
  , on_player_move_fn_(on_player_move_fn)
  , window_(window)
{
  model_ = glm::mat4(1.0f);
  model_ = glm::translate(model_, pos);
  model_ = glm::scale(model_, scale);

  bl_vertex_.x *= scale_.x;
  bl_vertex_.y *= scale_.y;

  tr_vertex_.x *= scale_.x;
  tr_vertex_.y *= scale_.y;
}

player_t::~player_t()
{
}

void player_t::draw(scene_state_t& scene_state)
{
  glUseProgram(texture_id());

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  model_ = glm::mat4(1.0f);
  auto last_pos = position_;
  glm::vec3 diff;

  if (controllable_) {
    auto c_b = [&](int state, float& v, float i) {
      auto curr_state = glfwGetKey(window_.window(), state);
      if (curr_state == GLFW_PRESS) {
        v += i;
      }
    };

    c_b(GLFW_KEY_W, position_.y,  0.01);
    c_b(GLFW_KEY_S, position_.y, -0.01);
    c_b(GLFW_KEY_D, position_.x,  0.01);
    c_b(GLFW_KEY_A, position_.x, -0.01);

    diff = position_ - last_pos;

    if (last_pos != position_) {
      scene_state.cam_vec.x += -(diff.x);
      scene_state.cam_vec.y += -(diff.y);
      on_player_move_fn_(id_, {position_.x, position_.y});
    } 
    //std::cout << "bl_player: X: " << bl.x << " Y: " << bl.y << std::endl; 
    //std::cout << "tr_player: X: " << tr.x << " Y: " << tr.y << std::endl; 
  }

  
  //std::cout << "bl_cam: X: " << bl_cam_box_.x << " Y: " << bl_cam_box_.y << std::endl; 
  //std::cout << "tr_cam: X: " << tr_cam_box_.x << " Y: " << tr_cam_box_.y << std::endl; 

 // std::cout << "scene_state: " << scene_state.cam_vec.x << " "
  //  << scene_state.cam_vec.y << std::endl;

  //std::cout << "position: " << position_.x << " "
  //  << position_.y << std::endl;
  

  if (controllable_) {
    //model_ = glm::translate(model_,
    //                        glm::vec3(-(diff.x), -(diff.y), 0.0f));
  }
  else {
    model_ = glm::translate(model_, position_);
    glm::vec3 c = glm::vec3(scene_state.cam_vec.x,
                            scene_state.cam_vec.y,
                            0.0f);
    model_ = glm::translate(model_, c);

    std::cout << "pos: " << position_.x << " " << position_.y << "\n";
    std::cout << scene_state.cam_vec.x << " " << scene_state.cam_vec.y 
      << "\n";
  }

  //model_ = glm::translate(model_, position_);
  model_ = glm::scale(model_, scale_);


  glUniformMatrix4fv(glGetUniformLocation(texture_id(), "model"),
                     1,
                     GL_FALSE,
                     glm::value_ptr(model_));

  glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f);

  if (controllable_) {
    colour = glm::vec3(1.0f, 0.5f, 0.0f);
  }
  else {
    colour = glm::vec3(1.0f, 1.0f, 1.0f);
  }

  glUniform4f(glGetUniformLocation(texture_id(), "player_colour"),
              colour.x,
              colour.y,
              colour.z,
              1.0f);

  draw_vertices();
}

void player_t::draw_vertices()
{
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
  glBindVertexArray(0);
}

void player_t::update_pos(glm::vec2 pos)
{
  position_ = {pos.x, pos.y, position_.z};
}