#include "scenes/shader.hpp"

#include <iostream>

using namespace multi2d;

shader_t::shader_t(const std::string vertex_code,
                   const std::string fragment_code)
  : id_(glCreateProgram())
{
  const auto vertex_c = compile_shader(vertex_code.c_str(),
                                       GL_VERTEX_SHADER);
  
  const auto fragment_c = compile_shader(fragment_code.c_str(),
                                         GL_FRAGMENT_SHADER);
  
  create_and_link_shader_program(vertex_c, fragment_c);
}

void shader_t::set_bool(const std::string& name, const bool v) const
{
  glUniform1i(glGetUniformLocation(id_, 
                                   name.c_str()), 
                                   static_cast<int>(v));
}

void shader_t::set_int(const std::string& name, const int v) const
{
  glUniform1i(glGetUniformLocation(id_, 
                                   name.c_str()), 
                                   v);
}

void shader_t::set_float(const std::string& name, const float v) const
{
  glUniform1f(glGetUniformLocation(id_, 
                                   name.c_str()), 
                                   v);
}

uint32_t shader_t::get_id() const
{
  return id_;
}

std::string shader_t::get_shader_source(const std::string& filename) const
{
  std::ifstream ifs;
  ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    ifs.open(filename);
    using buf_it = std::istreambuf_iterator<char>;
    return std::string(buf_it(ifs), buf_it());
  }
  catch (const std::ifstream::failure& e) {
    RUNTIME_THROW(status_t::IO_ERROR,
      "Failed getting shader source at filename '%s': '%s'",
      filename,
      e.what());
  }
}

uint32_t shader_t::compile_shader(const char* shader_code, 
                                  const uint32_t shader_type) const
{
  const auto shader = glCreateShader(shader_type);

  glShaderSource(shader, 1, &shader_code, NULL);
  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    char info_log[512];
    glGetShaderInfoLog(shader, 512, NULL, info_log);
    
    RUNTIME_THROW(status_t::OPENGL_FAILURE,
      "Failed to compile shader: '%s'",
      info_log);
  }

  return shader;
} 

template<typename ...ShaderType>
void shader_t::create_and_link_shader_program(const ShaderType... shaders)
{
  (glAttachShader(id_, shaders), ...);
  link_shader_program();
  (glDeleteShader(shaders), ...);
}

void shader_t::link_shader_program() const
{
  glLinkProgram(id_);
  
  int success;
  glGetProgramiv(id_, GL_LINK_STATUS, &success);
  if (!success) {
    char info_log[512];
    glGetProgramInfoLog(id_, 512, NULL, info_log);

    RUNTIME_THROW(status_t::OPENGL_FAILURE,
      "Failed to link shader: '%s'",
      info_log);
  }
}
