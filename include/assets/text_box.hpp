#ifndef TEXT_BOX_HPP
#define TEXT_BOX_HPP

#include <glm/ext.hpp>

#include "assets/rectangle.hpp"
#include "assets/bitmap_font.hpp"
#include "client/window.hpp"
#include "client/key_cb.hpp"
#include "scenes/scene.hpp"

namespace multi2d {

  class text_box_t : public rectangle_t
  {
  public:

    text_box_t(bitmap_font_t&   bitmap_font,
               window_t&        window,
               const uint32_t   shader_id,
               const glm::vec3  position,
               glm::vec3        scale  = glm::vec3(1.0f, 1.0f, 1.0f),
               glm::vec4        colour = glm::vec4(0.9f, 0.9f, 0.9f, 0.4f));

    std::string text() const;

    void draw(scene_state_t& scene_state) override;

  private:

    bitmap_font_t& bitmap_font_;
    window_t&      window_;
    std::string    input_text_;

    glm::vec2      bl_norm_dc_;
    glm::vec2      tr_norm_dc_;
    glm::vec3      colour_;

  };

}

#endif