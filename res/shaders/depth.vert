#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
uniform vec3 lightPos;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}