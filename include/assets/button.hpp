#ifndef BUTTON_T_HPP
#define BUTTON_T_HPP

#include <glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

#include "assets/rectangle.hpp"
#include "assets/bitmap_font.hpp"

namespace multi2d {

  struct button_t : public rectangle_t
  {
  public:

    button_t(const char*        label,
             window_t&          window,
             const uint32_t     shader,
             const glm::vec3    position,
             const std::string& text,
             bitmap_font_t&     bitmap_font,
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
    bitmap_font_t& bitmap_font_;
    window_t&      window_;

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

#endif
