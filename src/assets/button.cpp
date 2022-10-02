#include "assets/button.hpp"

using namespace multi2d;

button_t::button_t(const char*        label,
                   window_t&          window,
                   const uint32_t     shader,
                   const glm::vec3    position,
                   const std::string& text,
                   bitmap_font_t&     bitmap_font,
                   on_clicked_fn_t    on_clicked_fn,
                   const glm::vec3    scale,
                   const glm::vec4    colour)
  : rectangle_t(shader)
  , label_(label)
  , pos_(position)
  , colour_(colour)
  , text_(text)
  , bitmap_font_(bitmap_font)
  , on_clicked_fn_(on_clicked_fn)
  , window_(window)
{
  last_window_size_ = window_.window_size();

  model_ = glm::mat4(1.0f);
  model_ = glm::translate(model_, position);
  model_ = glm::scale(model_, scale);

  std::array<glm::vec4, 2> norm_dcs;

  for (auto i = 0; i < 12; i += 6) {
    glm::vec3 coord {rectangle[i],
                     rectangle[i + 1],
                     rectangle[i + 2]};
    
    glm::vec4 tsd(model_ * glm::vec4(coord, 1.0f));
    norm_dcs[i / 6] = tsd;
  }

  norm_dc_boundaries_.bottom_left = {norm_dcs[0].x,
                                     norm_dcs[0].y};

  norm_dc_boundaries_.top_right = {norm_dcs[1].x,
                                   norm_dcs[1].y};

  auto ws = window.window_size();
  auto window_width = ws.first;
  auto window_height = ws.second;

  auto c_b = [&](glm::vec4& v) -> glm::vec2 {
    return {
      ((v.x + 1.0) / 2.0) * window_width,
      (1.0 - ((v.y + 1.0) / 2.0)) * window_height
    };
  };

  screen_coord_boundaries_.bottom_left = c_b(norm_dcs[0]);
  screen_coord_boundaries_.top_right = c_b(norm_dcs[1]);
}

button_t::~button_t()
{
  glfwSetCursor(window_.window(), nullptr);
}

const char* button_t::label() const
{
  return label_;
}

void button_t::draw(scene_state_t& scene_state)
{
  set_texture();

  glUniformMatrix4fv(glGetUniformLocation(texture_id(), "model"),
                     1,
                     GL_FALSE,
                     glm::value_ptr(model_));

  glUniform4f(glGetUniformLocation(texture_id(), "button_colour"),
              colour_.x,
              colour_.y,
              colour_.z,
              colour_.w);
  
  draw_vertices();

  auto curr_window_size = window_.window_size();
  auto [last_width, last_height] = last_window_size_;

  if (curr_window_size != last_window_size_) {
    update_boundaries_ndc(last_width,
                          last_height,
                          curr_window_size.first,
                          curr_window_size.second);

    last_window_size_ = curr_window_size;
  }

  auto curr_cursor_pos = window_.cursor_pos();
  const auto [x, y] = curr_cursor_pos;

  auto l_click_state = glfwGetMouseButton(window_.window(),
                                          GLFW_MOUSE_BUTTON_LEFT);
  
  bool click_ready = (l_click_state == GLFW_RELEASE &&
    last_click_state_ == GLFW_PRESS);
  
  const bool inside = is_inside(x, y);

  if (click_ready && inside) {
    on_clicked_fn_();
  }

  last_click_state_ = l_click_state;

  if (last_cursor_pos_ != curr_cursor_pos) {
    static bool selected = false;

    if (inside) {
      selected = true;
      set_colour({0.6, 0.6, 0.6, 1});
      static auto c = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
      glfwSetCursor(window_.window(), c);
    }
    else {
      set_colour({0.4, 0.4, 0.4, 1});
      if (selected == false) {
        glfwSetCursor(window_.window(), nullptr);
      }
      selected = false;
    }

    last_cursor_pos_ = curr_cursor_pos;
  }

  if (!text_.empty()) {
    auto& b = norm_dc_boundaries_;

    bitmap_font_.print(text_,
                       norm_dc_boundaries_.bottom_left,
                       norm_dc_boundaries_.top_right);
  }
}

void button_t::update_boundaries_ndc(const float last_width,
                                     const float last_height,
                                     const float width,
                                     const float height)
{
  auto c_b = [&](glm::vec2& v) -> glm::vec2 {
    return {
      (v.x / last_width) * width,
      (v.y / last_height) * height
    };
  };

  screen_coord_boundaries_.bottom_left 
    = c_b(screen_coord_boundaries_.bottom_left);

  screen_coord_boundaries_.top_right 
    = c_b(screen_coord_boundaries_.top_right);
}

bool button_t::is_inside(const float x_pos, 
                         const float y_pos) const
{  
  auto& b = screen_coord_boundaries_;
  return (x_pos > b.bottom_left.x && x_pos < b.top_right.x)
    && (y_pos < b.bottom_left.y && y_pos > b.top_right.y);
}

void button_t::set_colour(glm::vec4 colour)
{
  colour_ = colour;
}
