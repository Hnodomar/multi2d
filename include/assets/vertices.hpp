#ifndef ASSET_VERTICES_HPP
#define ASSET_VERTICES_HPP

#include <functional>
#include <cstdint>
#include <tuple>
#include <optional>
#include <glad.h>

#include "util/exception.hpp"
#include "util/types.hpp"

namespace multi2d {

  class vertices_t
  {    
    using vertices_tuple_t = std::tuple<uint32_t,
                                        uint32_t,
                                        uint32_t>;

    using vertices_fn_t = std::function<vertices_tuple_t()>;
  public: 
    vertices_t(vertices_fn_t vertices_fn)
    {
      auto [vbo_, ebo_, vao_] = vertices_fn();
      vao = vao_;
      vbo = vbo_;
      ebo = ebo_;
    }

    ~vertices_t()
    {
      glDeleteVertexArrays(1, &vao);
      glDeleteBuffers(1, &vbo);
      glDeleteBuffers(1, &ebo);
    }
  
    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;
  };  
}

#endif