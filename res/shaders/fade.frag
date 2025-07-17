#version 410 core

in vec2 TexCoord;
out vec4 FragColor;

uniform float fade;
uniform vec4 col1;
uniform vec4 col2;

void main() {
    FragColor = vec4(0.0, 0.0, 0.0, fade);
}