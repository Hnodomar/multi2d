#include "scenes/scene.hpp"

using namespace multi2d;

scene_t::scene_t(const shader_t shader, window_t& window)
  : shader_(shader)
  , window_(window)
{

}

scene_t::~scene_t() 
{
}

void scene_t::add_asset(const char* group, std::unique_ptr<asset_t> asset)
{
  asset->enable();
  assets_[group].emplace_back(std::move(asset));
}

uint32_t scene_t::shader_id() const
{
  return shader_.get_id();
}

void scene_t::enable_group(const char* group)
{
  auto it = assets_.find(group);

  if (it == assets_.end()) {
    RUNTIME_THROW(status_t::INVALID_ARG,
      "Tried enabling asset group %s which does not exist",
      group);
  }

  for (auto& asset : it->second) {
    asset->enable();
  }
}

scene_state_t& scene_t::state()
{
  return state_;
}