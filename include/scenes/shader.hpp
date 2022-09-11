#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad.h>
#include <cstdint>
#include <fstream>

#include "util/exception.hpp"

namespace multi2d {
  class shader_t
  {
  public:

    shader_t(const std::string vertex_code, 
             const std::string fragment_path);

    void activate_shader() const;

    void set_bool(const std::string& name, const bool v) const;
    
    void set_int(const std::string& name, const int v) const;

    void set_float(const std::string& name, const float v) const;

    uint32_t get_id() const;

  private:

    std::string get_shader_source(const std::string& filename) const;

    uint32_t compile_shader(const char* shader_code, 
                            const uint32_t shader_type) const;

    template<typename ...ShaderType>
    void create_and_link_shader_program(const ShaderType...);

    void link_shader_program() const;

    const uint32_t id_;
  };
}

#endif