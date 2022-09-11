#ifndef SCENE_ASSETS_HPP
#define SCENE_ASSETS_HPP

#include <array>
#include <cstdint>
#include <string>
#include <functional>
#include <glad.h>
#include <glm/glm.hpp>
#include "glm.hpp"
#include <iostream>
#include <glm/ext.hpp>

namespace multi2d {
  template<typename v, typename... e>
  constexpr auto array_of(e&& ...t)
    -> std::array<v, sizeof...(e)>
  {
    return {{std::forward<e>(t)...}};
  }

  namespace assets 
  {

    using on_draw_fn_t = std::function<void()>; 
      
    struct asset_t
    {
      virtual ~asset_t() {}

      virtual void draw() = 0;

      virtual void draw_vertices() = 0;
    };

    namespace geometry 
    {      

      const auto rectangle = array_of<float>(
        -0.5f, -0.15f, 0.0f,
         0.5f, -0.15f, 0.0f,
         0.5f,  0.15f, 0.0f,
        -0.5f,  0.15f, 0.0f,
         0.5f,  0.15f, 0.0f,
        -0.5f, -0.15f, 0.0f
      );

      struct rectangle_t : public asset_t
      {
        virtual ~rectangle_t() {}

        static void init_rectangle();

        static void destroy_rectangle();

        void draw_vertices() override;
      
      private:

        static uint32_t vbo_;
        static uint32_t vao_;
      };
      
    }
    namespace menus 
    {

      struct button_t : public geometry::rectangle_t
      {
      public:

        button_t(const char*        label,
                 const uint32_t     window_width,
                 const uint32_t     window_height,
                 const uint32_t     shader,
                 const glm::vec3    position,
                 const std::string& text,
                 const glm::vec3    scale = glm::vec3(1.0f, 1.0f, 1.0f),
                 const glm::vec4    colour = glm::vec4(0.4, 0.4, 0.4, 1));

        struct boundaries;
        boundaries const& boundaries() const;

        void draw() override;

        const char* label() const;

        bool is_inside(const float x_pos,
                       const float y_pos) const;

        void update_boundaries_ndc(const float last_width,
                                   const float last_height,
                                   const float width,
                                   const float height);

        void set_colour(glm::vec4 colour);
        
      private:

        const char*    label_;
        const uint32_t shader_;
        glm::vec3      pos_;
        glm::vec4      colour_;
        glm::mat4      model_;
        std::string    text_;

        struct boundaries
        {
          glm::vec2 top_left;
          glm::vec2 top_right;
          glm::vec2 bottom_left;
          glm::vec2 bottom_right;

          void output()
          {
            auto o = [&](const std::string& n, glm::vec2& v) {
              std::cout << n << " x: " << v.x << " y: " << v.y << std::endl;
            };

            o("top left", top_left);
            o("top right", top_right);
            o("bottom left", bottom_left);
            o("bottom right", bottom_right);
          }
          
        } boundaries_;

      };
    }
  }
}


#endif