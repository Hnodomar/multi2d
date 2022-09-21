R"(
#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 text_colour;

void main() {
  color = texture(text, TexCoords) * vec4(text_colour, 1.0);
}
)"