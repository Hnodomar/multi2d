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

  auto& main = assets_["main"];

  main.emplace_back(
    std::make_unique<button_t>(
      "host-game",
      window_,
      shader_.get_id(),
      glm::vec3(-0.5f, -0.5f, 0.0f),
      "HOST GAME",
      bitmap_font_,
      [&]() {
        disable_assets("main");
        enable_assets("host_screen");
      },
      glm::vec3(0.5f, 0.5f, 0.5f)
    )
  );

  main.emplace_back(
    std::make_unique<button_t>(
      "join-game",
      window_,
      shader_.get_id(),
      glm::vec3(0.5f, -0.5f, 0.0f),
      "JOIN GAME",
      bitmap_font_,
      [&]() {
        disable_assets("main");
        enable_assets("join_screen");
      },
      glm::vec3(0.5f, 0.5f, 0.5f)
    )
  );

  auto& host_screen = assets_["host_screen"];
  host_screen.emplace_back(
    std::make_unique<text_input_box_t>(
      window_,
      bitmap_font_,
      "ENTER PORT TO HOST ON",
      shader_.get_id(),
      [&]() {
        enable_assets("main");
        disable_assets("host_screen");
      },
      on_click_host_game_fn,
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(1.2f, 1.6f, 1.2f)
    )
  );

  auto& join_screen = assets_["join_screen"];
  join_screen.emplace_back(
    std::make_unique<text_input_box_t>(
      window_,
      bitmap_font_,
      "ENTER IP ADDRESS",
      shader_.get_id(),
      [&]() {
        enable_assets("main");
        disable_assets("join_screen");
      },
      on_click_join_game_fn,
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(1.2f, 1.6f, 1.2f)
    )
  );

  for (auto& asset : main) {
    asset->enable();
  }
}

void menu_scene_t::enable_assets(const char* group)
{
  for (auto& asset : assets_[group]) {
    asset->enable();
  }
}

void menu_scene_t::disable_assets(const char* group)
{
  for (auto& asset : assets_[group]) {
    asset->disable();
  }
}

menu_scene_t::~menu_scene_t()
{
  rectangle_t::destroy_rectangle();
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(0);
}

void menu_scene_t::draw_scene() 
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  auto [x, y] = window_.window_size();
  bg_image_.draw(x, y);

  for (auto& [gn, ag] : assets_) {
    for (auto& a : ag) {
      a->draw_asset();
    }
  }

  draw_title();
}

void menu_scene_t::draw_title()
{
  bitmap_font_.print("MULTIPLAYER  2D",
                     {-0.5, 0.4},
                     { 0.5, 0.9});
}