#include "assets/bitmap_font.hpp"

#include <iostream>

using namespace multi2d;

bitmap_font_state_t::bitmap_font_state_t()
  : shader(bmp_vertex_code, bmp_fragment_code)
{
  shader.activate_shader();

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  std::array<uint32_t, 6> indices = {
    0, 1, 3,
    1, 2, 3
  };

  std::array<float, 32> vertices = {0};
  glBufferData(GL_ARRAY_BUFFER, 
               vertices.size() * sizeof(float),
               vertices.data(),
               GL_STATIC_DRAW);


  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               indices.size() * sizeof(uint32_t),
               indices.data(),
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

bitmap_font_t::bitmap_font_t(std::string_view sv,
                             window_t&        window)
  : window_(window)
{
  load_bitmap(sv);
}

uint32_t bitmap_font_t::texture_id() const
{
  return texture_id_;
}

void bitmap_font_t::load_bitmap(std::string_view sv)
{
  std::ifstream ifs;
  ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    ifs.open(sv.data(), 
             std::ios_base::binary | std::ios_base::in);
  }
  catch (const std::ifstream::failure& e) {
    RUNTIME_THROW(status_t::IO_ERROR,
      "Failed getting bitmap at filename '%s': '%s'",
      sv,
      e.what());
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
    RUNTIME_THROW(status_t::IO_ERROR,
      "Failed reading bitmap binary '%s': '%s'",
      sv,
      e.what());
  }

  ifs.close();

  std::memcpy(&header_, data.data(), sizeof(bitmap_header_t));

  // ID must be 'BFF2' for bitmap fonts
  if (header_.bitmap_id != 0xF2BF) {
    RUNTIME_THROW(status_t::IO_ERROR,
      "Invalid ID ('%u') for bitmap font '%s', unsupported format",
      header_.bitmap_id,
      sv.data());
  }

  size_t n_bytes = (header_.image_height * header_.image_width)
    * (header_.bits_per_pixel / 8);

  size_t expected_size = sizeof(bitmap_header_t) + n_bytes; 

  if (file_size != expected_size) {
    RUNTIME_THROW(status_t::IO_ERROR,
      "Unexpected bitmap filesize. Bitmap data expects '%lu'",
      "but is actually '%lu'",
       expected_size,
       file_size);
  }

  cells_per_row_ = header_.image_width / header_.cell_width;

  cell_width_ratio_ = (float)header_.cell_width 
    / (float)header_.image_width;

  cell_height_ratio_ = (float)header_.cell_height
    / (float)header_.image_height;

  switch (header_.bits_per_pixel) {
    case 8:
      render_style_ = BFG_RS_ALPHA;
      break;
    case 24:
      render_style_ = BFG_RS_RGB;
      break;
    case 32:
      render_style_ = BFG_RS_RGBA;
      break;
    default:
      RUNTIME_THROW(status_t::IO_ERROR,
        "Unrecognised bits per pixel value: '%u' for '%s'",
        header_.bits_per_pixel,
        sv.data());
  }

  image_.reserve(n_bytes);
  auto img_begin_it = data.data() + sizeof(bitmap_header_t);
  std::memcpy(image_.data(), 
              img_begin_it, 
              n_bytes);

  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); 

  // Fonts should be rendered at native resolution so no need for texture filtering
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

  // Stop chararcters from bleeding over edges
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

  auto r_fn = [&](const int blend_flag) {
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 blend_flag,
                 header_.image_width,
                 header_.image_height,
                 0,
                 blend_flag,
                 GL_UNSIGNED_BYTE,
                 image_.data());
  };

  switch (render_style_) {
    case BFG_RS_ALPHA:
      r_fn(GL_LUMINANCE);
      break;
    case BFG_RS_RGB:
      r_fn(GL_RGB);
      break;
    case BFG_RS_RGBA:
      r_fn(GL_RGBA);
      break;
  }
}

void bitmap_font_t::set_colour(const float red,
                               const float green,
                               const float blue)
{
  red_ = red;
  green_ = green;
  blue_ = blue;
}

// Sets up an Ortho screen based on the supplied values 
void bitmap_font_t::set_screen(const float x, const float y)
{
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, x, 0, y, -1, 1);
  glMatrixMode(GL_MODELVIEW);
/*
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  if (invert_y_axis_) {
    glOrtho(0, x, y, 0, -1, 1);
  }
  else {
    glOrtho(0, x, 0, y, -1, 1);
  }

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();*/
}

void orth_start()
{
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0f, 800, 600, 0.0f, 0.0f, 1.0f);
  glMatrixMode(GL_MODELVIEW);
}

void orth_end()
{
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

void bitmap_font_t::print(const std::string& text,
                          const float        x_lower,
                          const float        x_upper,
                          const float        y_lower,
                          const float        y_upper)
{
  if (text.length() > 255) {
    RUNTIME_THROW(status_t::INVALID_ARG,
      "bitmap_font::print() with text length > 255: '%s'",
      text);
  }

  glClear(GL_DEPTH_BUFFER_BIT);
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture_id_);

  bmp_font_state_.shader.activate_shader();

  glBindVertexArray(bmp_font_state_.vao);
  glBindBuffer(GL_ARRAY_BUFFER, bmp_font_state_.vbo);

  const size_t len = text.length();

  const float dy =
    static_cast<float>(header_.cell_height) / 
      static_cast<float>(window_.window_size().second);
  
  print_location_x_ = x_lower;

  for (size_t i = 0; i != len; ++i) {
    const int row = (text[i]- header_.base) / cells_per_row_;
    const int col = (text[i] - header_.base) - row * cells_per_row_;

    const float ratio = (float)header_.cell_height / 
                         (float)header_.char_widths[text[i]];

    const float u = col * cell_width_ratio_;
    const float v = row * cell_height_ratio_;
    const float u1 = u + cell_width_ratio_;
    const float v1 = v + cell_height_ratio_;

    float dx = 
      (2.0 / (float)window_.window_size().first) * 
        (float)header_.char_widths[text[i]];

    std::array<float, 32> vertices = {
      print_location_x_, y_lower, 0.0f,
      1.0f, 0.0f, 0.0f,
      u, v1,

      print_location_x_ + 0.1, y_lower, 0.0f,
      0.0f, 1.0f, 0.0f,
      u1, v1,

      print_location_x_ + 0.1, y_upper, 0.0f,
      0.0f, 0.0f, 1.0f,
      u1, v,

      print_location_x_, y_upper, 0.0f,
      1.0f, 1.0f, 0.0f,
      u, v
    };

    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    vertices.size() * sizeof(float),
                    vertices.data());

    print_location_x_ += dx;

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
  }
}

void bitmap_font_t::set_blend()
{
  glColor3f(red_, green_, blue_);

  switch (render_style_) {
    case BFG_RS_ALPHA: // 8Bit
      glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
      glEnable(GL_BLEND);
      break;
    case BFG_RS_RGB:   // 24Bit
      glDisable(GL_BLEND);
      break;
    case BFG_RS_RGBA:  // 32Bit
      glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);
      break;
  }
}

void bitmap_font_t::set_print_location(const float x, const float y)
{
  print_location_x_ = x;
  print_location_y_ = y;
}

size_t bitmap_font_t::get_pixel_width(const std::string& s) const
{
  size_t ret = 0;

  for (size_t i = 0; i < s.length(); ++i) {
    ret += header_.char_widths[s[i]];
  }

  return ret;
}
