R"(
#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;

void main() {
  color = texture(text, TexCoords);
}
)"