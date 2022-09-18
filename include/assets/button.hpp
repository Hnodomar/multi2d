#ifndef BUTTON_T_HPP
#define BUTTON_T_HPP

#include <glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <functional>

#include "assets/rectangle.hpp"
#include "assets/bitmap_font.hpp"

namespace multi2d {

  using on_clicked_fn_t = std::function<void ()>;

  struct button_t : public rectangle_t
  {
  public:

    button_t(const char*        label,
             window_t&          window,
             const uint32_t     shader,
             const glm::vec3    position,
             const std::string& text,
             bitmap_font_t&     bitmap_font,
             on_clicked_fn_t    on_clicked_fn,
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

    const char*     label_;
    const uint32_t  shader_;
    glm::vec3       pos_;
    glm::vec4       colour_;
    glm::mat4       model_;
    std::string     text_;
    bitmap_font_t&  bitmap_font_;
    on_clicked_fn_t on_clicked_fn_;
    window_t&       window_;
    
    std::pair<uint32_t, uint32_t> last_window_size_ = {0, 0};
    std::pair<double, double>     last_cursor_pos_ = {0.0, 0.0};
    int                           last_click_state_ = GLFW_RELEASE;

    struct boundary_t
    {
      glm::vec2 top_right;
      glm::vec2 bottom_left;

      void output()
      {
        auto o = [&](const std::string& n, glm::vec2& v) {
          std::cout << n << " x: " << v.x << " y: " << v.y << std::endl;
        };

        o("top right", top_right);
        o("bottom left", bottom_left);
      }
      
    }; 
    
    boundary_t screen_coord_boundaries_;

    boundary_t norm_dc_boundaries_;
  };

}

#endif
