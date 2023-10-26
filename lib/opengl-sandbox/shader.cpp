/**
 * @file shader.cpp
 * @brief Shader class supporting vertex and fragment shaders
 * @date October 2023
 */

#include <opengl-sandbox/shader.h>

Shader::Shader(const char* vertexPath, const char* fragmentPath, Logger* logger)
	: logger(logger) {
	std::string vShaderCode, fShaderCode;
	std::ifstream vShaderFile, fShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		std::stringstream vShaderStream, fShaderStream;
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vShaderCode = vShaderStream.str();
		fShaderCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e) {
		if (this->logger)
			this->logger->log("ERROR::SHADER::FILE_READ_UNSUCCESSFULLY\n" + std::string{e.what()});
	}
	const char* vertexCode = vShaderCode.c_str();
	const char* fragmentCode = fShaderCode.c_str();
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode);
	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);
	checkCompileErrors(id);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader() {
	glDeleteProgram(id);
}

void Shader::use() const {
	glUseProgram(id);
}

void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
	glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string& name, float x, float y) const {
	glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
	glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const {
	glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string& name, const glm::mat2& mat) const {
	glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) const {
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

GLuint Shader::compileShader(GLenum shaderType, const GLchar* shaderCode) const {
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderCode, nullptr);
	glCompileShader(shader);
	checkCompileErrors(shader, shaderType);
	return shader;
}

void Shader::checkCompileErrors(GLuint shader, GLenum shaderType) const {
	GLint success;
	GLchar infoLog[INFO_LOG_SIZE];
	if (shaderType) {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success && logger) {
			glGetShaderInfoLog(shader, INFO_LOG_SIZE, nullptr, infoLog);
			logger->log("ERROR::SHADER::" + shaderTypeToString(shaderType) + "::COMPILATION_FAILED\n" + infoLog);
		}
	}
	else {
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success && logger) {
			glGetProgramInfoLog(id, INFO_LOG_SIZE, nullptr, infoLog);
			logger->log("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + std::string{infoLog});
		}
	}
}

std::string Shader::shaderTypeToString(GLenum shaderType) const {
	switch (shaderType) {
		case GL_VERTEX_SHADER:
			return "VERTEX";
		case GL_FRAGMENT_SHADER:
			return "FRAGMENT";
		default:
			return "UNRECOGNIZED_SHADER_TYPE";
	}
}