#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <iostream>
#include <stdexcept>
#include <format>

#include "util.hpp"
#include "shader.hpp"

void Shader::checkCompileErrors(unsigned int shader, enum ShaderType type) {
	int success;
	char infoLog[1024];

	if (type != PROGRAM) {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
			throw std::runtime_error(std::format("Error compiling {} shader.\n{}\n", 
				!type ? "program" : type == 1 ? "vertex" : "fragment", infoLog
			));
		}
	} else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
			throw std::runtime_error(std::format("Error linking {} shader.\n{}\n", 
				!type ? "program" : type == 1 ? "vertex" : "fragment", infoLog
			));
		} 
	}
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
	std::ifstream vsf(vertexPath);
	std::ifstream fsf(fragmentPath);

	if (!vsf) throw std::runtime_error(std::format("Failed to locate vertex path at {}\n", vertexPath));
	if (!fsf) throw std::runtime_error(std::format("Failed to locate fragment path at {}\n", fragmentPath));

	std::string vertexShaderSource = util::slurp(vsf);
	std::string fragmentShaderSource = util::slurp(fsf);

	const char* vertexShaderCode = vertexShaderSource.c_str();
	const char* fragmentShaderCode = fragmentShaderSource.c_str();
	unsigned int vertexShader, fragmentShader;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const char* const*) &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
	checkCompileErrors(vertexShader, VERTEX);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const char* const*) &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);  
	checkCompileErrors(fragmentShader, FRAGMENT);

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertexShader);
	glAttachShader(m_ID, fragmentShader);
	glLinkProgram(m_ID);
	checkCompileErrors(m_ID, PROGRAM);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use() {
  	glUseProgram(m_ID);
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, glm::vec2 value) const {
	glUniform2f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y); 
}

void Shader::setVec3(const std::string& name, glm::vec3 value) const {
	glUniform3f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z); 
}

void Shader::setVec4(const std::string& name, glm::vec4 value) const {
	glUniform4f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::setMat4(const std::string& name, glm::mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}