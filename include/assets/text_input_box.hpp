#ifndef TEXT_INPUT_BOX_HPP
#define TEXT_INPUT_BOX_HPP

#include <cstdint>
#include <glm/ext.hpp>
#include <vector>
#include <array>
#include <memory>

#include "assets/asset.hpp"
#include "assets/bitmap_font.hpp"
#include "assets/button.hpp"
#include "assets/rectangle.hpp"
#include "assets/text_box.hpp"

namespace multi2d {

  class text_input_box_t : public rectangle_t
  {
  public:
    text_input_box_t(window_t&       window,
                     bitmap_font_t&  bitmap_font,
                     std::string     title,
                     const uint32_t  shader_id,
                     on_clicked_fn_t on_back_clicked,
                     on_clicked_fn_t on_ok_clicked,
                     glm::vec3       pos,
                     glm::vec3       scale);

    virtual ~text_input_box_t();

    void draw() override;
        
  private:

    glm::mat4      model_;
    glm::vec3      position_;
    glm::vec3      scale_;
    glm::vec2      bl_norm_dc_;
    glm::vec2      tr_norm_dc_;

    using asset_ptr_t = std::unique_ptr<asset_t>;

    std::array<asset_ptr_t, 3> assets_;

    bitmap_font_t& bitmap_font_;
    window_t&      window_;
    std::string    title_;
  };

}

#endif