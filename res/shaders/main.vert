#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
	TexCoord = aTexCoord;

	mat3 normalMatrix = transpose(inverse(mat3(model)));

	Normal = normalize(normalMatrix * aNormal);
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
	//inv_mvp = inverse(projection * view * model);

	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}