#ifndef MENU_SCENE_HPP
#define MENU_SCENE_HPP

#include <functional>
#include <vector>
#include <glm.hpp>

#include "scenes/scene.hpp"
#include "scenes/assets.hpp"
#include "client/window.hpp"

namespace multi2d {

  using on_click_join_game_t = std::function<void()>;

  using on_click_host_game_t = std::function<void()>;

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
                 window_t&            window);

    virtual ~menu_scene_t();

    void draw_scene() override;

    void process_input() override;

  private:

    void adjust_ndc();

    void mouseover_check(const double x, const double y);

    void mousepress_check(const double x, const double y);
    
    on_click_join_game_t  on_click_join_game_fn_;
    on_click_host_game_t  on_click_host_game_fn_;

    std::vector<assets::menus::button_t> buttons_;
    std::pair<double, double>            last_cursor_pos_ = {0.0, 0.0};
    std::pair<uint32_t, uint32_t>        last_window_size_ = {0, 0};
    int                                  last_click_state_ = GLFW_RELEASE;
  };
}

#endif