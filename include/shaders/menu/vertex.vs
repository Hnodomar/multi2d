R"(
#version 330 core

layout (location = 0) in vec3 button_coords;

uniform mat4 model;

void main() {
  gl_Position = model * vec4(button_coords, 1.0);
}
)"