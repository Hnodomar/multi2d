R"(
#version 330 core

out vec4 frag_colour;

uniform vec4 player_colour;

void main() {
  frag_colour = player_colour;
}
)"