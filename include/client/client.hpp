#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <optional>
#include <memory>

#include "client/window.hpp"
#include "multi/tcp_server.hpp"
#include "multi/tcp_connection.hpp"
#include "util/event_loop.hpp"
#include "scenes/scene.hpp"
#include "scenes/menu.hpp"
#include "scenes/assets/bitmap_font.hpp"

namespace multi2d {

  class multi2d_client_t
  {
  public:
    
    multi2d_client_t(const int width, const int height);

    void start();

  private:

    void set_scene(std::unique_ptr<scene_t> scene);

    void render_loop();

    window_t                        window_;
    std::unique_ptr<scene_t>        scene_;
    std::optional<tcp_connection_t> connection_;
    std::optional<tcp_server_t>     server_;
    event_loop_t                    ev_loop_;
    bitmap_font_t                   bitmap_font_;
  };

}

#endif