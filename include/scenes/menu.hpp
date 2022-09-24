#ifndef MENU_SCENE_HPP
#define MENU_SCENE_HPP

#include <functional>
#include <vector>
#include <string>
#include <glm.hpp>

#include "scenes/scene.hpp"
#include "assets/bitmap_font.hpp"
#include "assets/button.hpp"
#include "client/window.hpp"
#include "assets/bg_image.hpp"
#include "assets/text_input_box.hpp"

namespace multi2d {

  using on_click_join_game_t = std::function<void(const std::string&)>;

  using on_click_host_game_t = std::function<void(const std::string&)>;

  const std::string menu_vertex_code = 
  #include "shaders/menu/vertex.vs"
  ;

  const std::string menu_fragment_code = 
  #include "shaders/menu/fragment.vs"
  ;

  class menu_scene_t : public scene_t
  {
  public:
    menu_scene_t(on_click_join_game_t on_click_join_game_fn,
                 on_click_host_game_t on_click_host_game_fn,
                 window_t&            window,
                 bitmap_font_t&       bitmap_font);

    virtual ~menu_scene_t();

    void draw_scene() override;

    void handle_packet(pkt_ref_t pkt);

  private:

    void enable_assets(const char* group);

    void disable_assets(const char* group);

    void draw_title();
    
    bitmap_font_t&        bitmap_font_;

    bg_image_t            bg_image_;

    bool host_clicked_ = false;
    bool join_clicked_ = false;

    std::pair<double, double>            last_cursor_pos_ = {0.0, 0.0};
    std::pair<uint32_t, uint32_t>        last_window_size_ = {0, 0};
    int                                  last_click_state_ = GLFW_RELEASE;
  };
}

#endif