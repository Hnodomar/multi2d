#ifndef ASSET_HPP
#define ASSET_HPP

#include <array>
#include <glad.h>

namespace multi2d {

  template <typename V, typename... T>
  constexpr auto array_of(T&&... t)
    -> std::array<V, sizeof...(T)>
  {
    return {{std::forward<T>(t)...}};
  }

  struct asset_t
  {
    virtual ~asset_t() {}

    virtual void draw() = 0;

    virtual void draw_vertices() = 0;
  };

}

#endif
