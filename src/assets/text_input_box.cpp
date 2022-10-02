#include "assets/text_input_box.hpp"

using namespace multi2d;

text_input_box_t::text_input_box_t(window_t&       window,
                                   bitmap_font_t&  bitmap_font,
                                   std::string     title,
                                   const uint32_t  shader_id,
                                   on_clicked_fn_t on_back_clicked,
                                   on_submit_fn_t  on_ok_clicked,
                                   glm::vec3       pos,
                                   glm::vec3       scale)
  : rectangle_t(shader_id)
  , position_(pos)
  , scale_(scale)
  , bitmap_font_(bitmap_font)
  , window_(window)
  , title_(title)
{
  model_ = glm::mat4(1.0f);
  model_ = glm::translate(model_, pos);
  model_ = glm::scale(model_, scale);

  std::array<glm::vec4, 2> norm_dcs;
  for (auto i = 0; i < 12; i += 6) {
    glm::vec3 coord {rectangle[i],
                     rectangle[i + 1],
                     rectangle[i + 2]};
    
    glm::vec4 tsd(model_ * glm::vec4(coord, 1.0f));
    norm_dcs[i / 6] = tsd;
  }

  bl_norm_dc_ = {norm_dcs[0].x, norm_dcs[0].y};
  tr_norm_dc_ = {norm_dcs[1].x, norm_dcs[1].y};

  auto x_interval = tr_norm_dc_.x - bl_norm_dc_.x;
  auto y_interval = tr_norm_dc_.y - bl_norm_dc_.y;

  glm::vec3 text_box_pos = pos;
  text_box_pos.y = tr_norm_dc_.y - (y_interval * 0.3);
  auto text_box = std::make_unique<text_box_t>(bitmap_font,
                                               window,
                                               shader_id,
                                               text_box_pos,
                                               glm::vec3(0.8f, 0.5f, 0.3f));

  glm::vec3 back_pos = pos;
  back_pos.x = bl_norm_dc_.x + (x_interval * 0.2);
  back_pos.y = bl_norm_dc_.y + (y_interval * 0.25);
  auto back_button = std::make_unique<button_t>("back-button",
                                                window,
                                                shader_id,
                                                back_pos,
                                                "BACK",
                                                bitmap_font,
                                                on_back_clicked,
                                                glm::vec3(0.3f, 0.5f, 0.3f));

  auto on_ok_clicked_fn = [&, on_ok_clicked, txt_box=text_box.get()](){
    try {
      on_ok_clicked(txt_box->text());
    }
    catch (const multi2d::exception_t& e) {
      std::string err_str(e.what());
      std::transform(err_str.begin(),
                     err_str.end(),
                     err_str.begin(),
                     ::toupper);

      error_str_ = "ERROR: " + err_str;
    }
  };

  glm::vec3 ok_pos = pos;
  ok_pos.x = tr_norm_dc_.x - (x_interval * 0.2);
  ok_pos.y = bl_norm_dc_.y + (y_interval * 0.25);
  auto ok_button = std::make_unique<button_t>("ok-button",
                                              window,
                                              shader_id,
                                              ok_pos,
                                              " OK ",
                                              bitmap_font,
                                              on_ok_clicked_fn,
                                              glm::vec3(0.3f, 0.5f, 0.3f));

  assets_[0] = std::move(text_box);
  assets_[1] = std::move(back_button);
  assets_[2] = std::move(ok_button);

  for (auto& a : assets_) {
    a->enable();
  }
}

text_input_box_t::~text_input_box_t()
{
}

void text_input_box_t::draw(scene_state_t& scene_state)
{ 
  set_texture();

  glUniformMatrix4fv(glGetUniformLocation(texture_id(), "model"),
                     1,
                     GL_FALSE,
                     glm::value_ptr(model_));

  glUniform4f(glGetUniformLocation(texture_id(), "button_colour"),
              0.4f,
              0.4f,
              0.4f,
              0.0f);

  draw_vertices();

  for (auto& asset : assets_) {
    asset->draw_asset(scene_state);
  }

  bitmap_font_.print(error_str_, 
                    {bl_norm_dc_.x, -0.5f}, 
                    {-bl_norm_dc_.x, bl_norm_dc_.y});
}