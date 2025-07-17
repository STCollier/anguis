#version 410 core

in vec2 TexCoord;
out vec4 FragColor;

uniform vec2 resolution;
uniform float time;

uniform vec3 col1;
uniform vec3 col2;

void main() {

    vec2 uv = TexCoord * 2.0 - 1.0;
    uv.x *= resolution.x / resolution.y;
    uv.y += 0.5;

    float lineWidth = (0.5 + 0.25 * sin(time)) * 0.1;
    float glowWidth = lineWidth * 5.0;

    float dist = abs(uv.y);

    float glow = smoothstep(glowWidth, 0.0, dist);
    float core = smoothstep(lineWidth, 0.0, dist);

    vec3 color = col1 * glow + col2 * core;
    FragColor = vec4(color, 1.0);
}