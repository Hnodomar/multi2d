#include "assets/bg_image.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace multi2d;

bg_image_vertex_buffer_t::bg_image_vertex_buffer_t()
  : shader_(bg_vertex_code, bg_fragment_code)
{
  shader_.activate_shader();

  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
  glGenBuffers(1, &ebo_);

  glBindVertexArray(vao_);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER,
               vertex_buffer_data_.size() * sizeof(float),
               vertex_buffer_data_.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               indices_.size() * sizeof(uint32_t),
               indices_.data(),
               GL_STATIC_DRAW);

  constexpr auto stride = 8 * sizeof(float);
  
  glVertexAttribPointer(0,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        stride,
                        (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        stride,
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        stride,
                        (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}

bg_image_vertex_buffer_t::~bg_image_vertex_buffer_t()
{
  glDeleteVertexArrays(1, &vao_);
  glDeleteBuffers(1, &vbo_);
  glDeleteBuffers(1, &ebo_);
}
/*
bitmap_header_t h;
std::vector<char> get_img(std::string_view sv)
{
  std::ifstream ifs;
  ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    ifs.open(sv.data(), 
             std::ios_base::binary | std::ios_base::in);
  }
  catch (const std::ifstream::failure& e) {
  }

  ifs.seekg(0, std::ios_base::end);
  const size_t file_size = ifs.tellg();
  ifs.seekg(0, std::ios_base::beg);

  std::vector<uint8_t> data;
  data.reserve(file_size);

  try {
    ifs.read(reinterpret_cast<char*>(data.data()), 
             file_size);
  }
  catch (const std::ifstream::failure& e) {
  }

  ifs.close();

  

  std::memcpy(&h, data.data(), sizeof(bitmap_header_t));


  const size_t n_bytes = file_size - sizeof(bitmap_header_t);
  std::vector<char> ret;

  ret.reserve(n_bytes);
  auto img_begin_it = data.data() + sizeof(bitmap_header_t);
  std::memcpy(ret.data(), 
              img_begin_it, 
              n_bytes);

  return ret;
}*/


bg_image_t::bg_image_t(std::string_view filename,
                       window_t&        window)
  : window_(window)
{   
  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_2D, texture_id_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_set_flip_vertically_on_load(true);

  int nr_channels;
  unsigned char* data = stbi_load(filename.data(), 
                            reinterpret_cast<int*>(&texture_width_), 
                            reinterpret_cast<int*>(&texture_height_), 
                            &nr_channels, 
                            0);

  

  if (!data) {
    RUNTIME_THROW(status_t::IO_ERROR,
      "Failed to load bg_image_t %s",
      filename.data());
  }
  //auto img = get_img("../Arial.bff");
  glTexImage2D(GL_TEXTURE_2D,
            0,
            GL_RGB,
            texture_width_,
            texture_height_,
           // h.image_width,
           // h.image_height,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
           // img.data());
            data);
  
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);
}

void bg_image_t::draw(const uint32_t x, const uint32_t y)
{
  glBindTexture(GL_TEXTURE_2D, texture_id_);

  vbuffer_state_.shader_.activate_shader();
  glBindVertexArray(vbuffer_state_.vao_);
  glDrawElements(GL_TRIANGLES,
                 6,
                 GL_UNSIGNED_INT,
                 0);  
}
