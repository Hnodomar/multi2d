#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <optional>
#include <array>
#include <memory>
#include <functional>

#include "client/window.hpp"
#include "multi/tcp_server.hpp"
#include "multi/tcp_connection.hpp"
#include "util/event_loop.hpp"
#include "scenes/scene.hpp"
#include "scenes/world.hpp"
#include "scenes/menu.hpp"
#include "assets/bitmap_font.hpp"
#include "assets/player.hpp"
#include "client/key_cb.hpp"
#include "multi/pkt_config.hpp"

namespace multi2d {

  class multi2d_client_t
  {
  public:
    
    multi2d_client_t(const int width, const int height);

    void start();

    std::optional<tcp_server_t>& server();

    std::optional<tcp_connection_t>& connection();

  private:

    void handle_client_read(pkt_ref_t pkt);

    void host_game(const std::string& port);

    void join_game(const std::string& addr_and_port);

    void set_scene(std::unique_ptr<scene_t> scene);

    void set_next_scene(std::unique_ptr<scene_t> scene);

    void render_loop();

    void make_player();

    window_t                        window_;
    std::unique_ptr<scene_t>        next_scene_;
    std::unique_ptr<scene_t>        scene_;
    std::optional<tcp_connection_t> connection_;
    std::optional<tcp_server_t>     server_;
    event_loop_t                    ev_loop_;
    bitmap_font_t                   bitmap_font_;
  };

}

#endif