#ifndef ASSET_HPP
#define ASSET_HPP

#include <array>
#include <glad.h>
#include <memory>
#include <vector>

namespace multi2d {

  template <typename V, typename... T>
  constexpr auto array_of(T&&... t)
    -> std::array<V, sizeof...(T)>
  {
    return {{std::forward<T>(t)...}};
  }

  struct asset_t
  {
    asset_t(const uint32_t t_id);

    virtual ~asset_t() {}

    void set_texture();

    uint32_t texture_id() const;

    void disable();

    void enable();

    void draw_asset();
    
  protected:

    virtual void draw() = 0;

    virtual void draw_vertices() = 0;

    bool enabled_ = false;

    static uint32_t curr_texture_id_;

    const uint32_t t_id_;

  };

}

#endif
