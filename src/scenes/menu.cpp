#include "scenes/menu.hpp"

using namespace multi2d;



menu_scene_t::menu_scene_t(on_click_join_game_t on_click_join_game_fn,
                           on_click_host_game_t on_click_host_game_fn,
                           window_t&            window,
                           bitmap_font_t&       bitmap_font)
  : scene_t(shader_t(menu_vertex_code,
                     menu_fragment_code), window)
  , on_click_join_game_fn_(on_click_join_game_fn)
  , on_click_host_game_fn_(on_click_host_game_fn)
  , bitmap_font_(bitmap_font)
{
  assets::geometry::rectangle_t::init_rectangle();

  glUseProgram(shader_.get_id());

  last_window_size_ = window_.window_size();

  const auto [window_width, window_height] = last_window_size_;

  buttons_.emplace_back("host-game",
                        window_width,
                        window_height,
                        shader_.get_id(),
                        glm::vec3(0.0f, 0.3f, 0.0f),
                        "",
                        glm::vec3(0.5f, 0.5f, 0.5f));
    
  buttons_.emplace_back("join-game",
                        window_width,
                        window_height,
                        shader_.get_id(),
                        glm::vec3(0.0f, -0.3f, 0.0f),
                        "",
                        glm::vec3(0.5f, 0.5f, 0.5f));
}

menu_scene_t::~menu_scene_t()
{
  assets::geometry::rectangle_t::destroy_rectangle();
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

}

void menu_scene_t::draw_scene() 
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  for (auto& button : buttons_) {
    button.draw();
  }
}

void menu_scene_t::process_input()
{
  adjust_ndc();

  auto curr_cursor_pos = window_.cursor_pos();
  const auto [x, y] = curr_cursor_pos;

  mousepress_check(x, y);

  if (last_cursor_pos_ == curr_cursor_pos) {
    return;
  }
  last_cursor_pos_ = curr_cursor_pos;
  
  mouseover_check(x, y);
}

void menu_scene_t::adjust_ndc()
{
  auto curr_window_size = window_.window_size();
  if (last_window_size_ != curr_window_size) {
    for (auto& button : buttons_) {
      button.update_boundaries_ndc(last_window_size_.first,
                                   last_window_size_.second,
                                   curr_window_size.first,
                                   curr_window_size.second);
    }
    
    last_window_size_ = curr_window_size;
  }
}

void menu_scene_t::mouseover_check(const double x, const double y)
{
  for (auto& button : buttons_) {
    if (button.is_inside(x, y)) {
      button.set_colour({0.6, 0.6, 0.6, 1});
    }
    else {
      button.set_colour({0.4, 0.4, 0.4, 1});
    }
  }
}

void menu_scene_t::mousepress_check(const double x, const double y)
{
  auto click_state = glfwGetMouseButton(window_.window(),
                                        GLFW_MOUSE_BUTTON_LEFT);
  
  bool ready = click_state == GLFW_RELEASE
    && last_click_state_ == GLFW_PRESS;

  if (ready) {
    for (auto& button : buttons_) {
      if (button.is_inside(x, y)) {
        if (button.label() == "host-game") {
          on_click_host_game_fn_();
        }
        else if (button.label() == "join-game") {
          on_click_join_game_fn_();
        }
      }
    }
  }

  last_click_state_ = click_state;
}