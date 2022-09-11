R"(
#version 330 core

out vec4 frag_colour;

uniform vec4 button_colour;

void main() {
  frag_colour = button_colour;
}
)"