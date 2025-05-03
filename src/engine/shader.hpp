#pragma once

#include <glm/glm.hpp>
#include <string>

enum ShaderType {
	PROGRAM = 0,
    VERTEX,
    FRAGMENT
};

class Shader {
	public:
		Shader(const std::string& vertexPath, const std::string& fragmentPath);

		void use();

		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec2(const std::string& name, glm::vec2 value) const;
		void setVec3(const std::string& name, glm::vec3 value) const;
		void setVec4(const std::string& name, glm::vec4 value) const;
		void setMat4(const std::string& name, glm::mat4 value) const;

	private:
		void checkCompileErrors(unsigned int shader, enum ShaderType type);

		unsigned int m_ID;
};