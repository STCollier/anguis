#version 410 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D sceneTexture;
uniform sampler2D bloomTexture;

uniform bool useBloom;
uniform float exposure;

vec3 ACES(vec3 x) {
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

void main()
{
    vec3 HDRColor = texture(sceneTexture, TexCoord).rgb;
    vec3 bloomColor = texture(bloomTexture, TexCoord).rgb;

    if (useBloom) HDRColor += bloomColor;

    vec3 result = ACES(HDRColor * exposure);
    FragColor = vec4(result, 1.0);
}