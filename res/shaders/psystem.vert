#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in mat4 instanceMatrix;
layout (location = 7) in vec4 aColor;

out vec3 Normal;
out vec3 FragPos;
out vec4 Color;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    FragPos = vec3(instanceMatrix * vec4(aPos, 1.0));
    Normal = transpose(inverse(mat3(instanceMatrix))) * aNormal;
    Color = aColor;

    gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
}