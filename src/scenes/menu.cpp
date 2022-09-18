#include "scenes/menu.hpp"

using namespace multi2d;

menu_scene_t::menu_scene_t(on_click_join_game_t on_click_join_game_fn,
                           on_click_host_game_t on_click_host_game_fn,
                           window_t&            window,
                           bitmap_font_t&       bitmap_font)
  : scene_t(shader_t(menu_vertex_code,
                     menu_fragment_code), window)
  , bitmap_font_(bitmap_font)
  , bg_image_("../extern/textures/main_menu_bg.png", window_)
{
  rectangle_t::init_rectangle();

  glUseProgram(shader_.get_id());

  last_window_size_ = window_.window_size();

  buttons_.emplace_back("host-game",
                        window_,
                        shader_.get_id(),
                        glm::vec3(-0.5f, -0.5f, 0.0f),
                        "HOST GAME",
                        bitmap_font_,
                        on_click_host_game_fn,
                        glm::vec3(0.5f, 0.5f, 0.5f));
    
  buttons_.emplace_back("join-game",
                        window_,
                        shader_.get_id(),
                        glm::vec3(0.5f, -0.5f, 0.0f),
                        "JOIN GAME",
                        bitmap_font_,
                        on_click_join_game_fn,
                        glm::vec3(0.5f, 0.5f, 0.5f));
}

menu_scene_t::~menu_scene_t()
{
  rectangle_t::destroy_rectangle();
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

}

void menu_scene_t::draw_scene() 
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  auto [x, y] = window_.window_size();
  bg_image_.draw(x, y);

  for (auto& button : buttons_) {
    button.draw();
  }

  draw_title();
}

void menu_scene_t::draw_title()
{
  bitmap_font_.print("MULTIPLAYER  2D",
                     {-0.5, 0.4},
                     { 0.5, 0.9});
}