#ifndef WORLD_HPP
#define WORLD_HPP

#include <iostream>
#include <memory>
#include <glad.h>
#include <functional>
#include <map>

#include "scenes/scene.hpp"
#include "client/window.hpp"
#include "assets/asset.hpp"
#include "assets/bitmap_font.hpp"
#include "assets/player.hpp"

namespace multi2d {

  using on_quit_fn_t = std::function<void()>;
  using on_pos_update_fn_t = std::function<void(const uint32_t,
                                                const glm::vec2)>;

  const std::string world_vertex_code = 
  #include "shaders/world/vertex.vs"
  ;

  const std::string world_fragment_code = 
  #include "shaders/world/fragment.vs"
  ;

  class world_scene_t : public scene_t
  {
  public:

    world_scene_t(window_t&           window,
                  on_quit_fn_t        on_quit_fn,
                  bitmap_font_t&      bitmap_font);

    virtual ~world_scene_t();

    void draw_scene() override;

    void handle_packet(pkt_ref_t pkt) override;

  private:

    void update_player_pos(int32_t id, glm::vec2 pos);

    on_quit_fn_t   on_quit_fn_;

    bitmap_font_t& bitmap_font_;

    using player_id = int;
    using player_ref_t = std::reference_wrapper<player_t>;
    
    std::map<player_id, player_ref_t> id_to_player_;
  };

}

#endif