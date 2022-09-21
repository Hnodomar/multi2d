#include "assets/rectangle.hpp"

using namespace multi2d;

uint32_t asset_t::curr_texture_id_ = 10000;

void asset_t::set_texture()
{
  if (curr_texture_id_ != t_id_) {
    curr_texture_id_ = t_id_;
    
  }
  glUseProgram(curr_texture_id_);
}

asset_t::asset_t(const uint32_t t_id)
  : t_id_(t_id)
{}

uint32_t asset_t::texture_id() const
{
  return t_id_;
}

void asset_t::enable()
{
  enabled_ = true;
}

void asset_t::disable()
{
  enabled_ = false;
}

void asset_t::draw_asset()
{
  if (enabled_) {
    draw();
  }
}