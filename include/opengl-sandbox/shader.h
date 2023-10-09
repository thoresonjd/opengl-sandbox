/**
 * @file shader.h
 * @brief Shader class supporting vertex and fragment shaders
 * @date October 2023
 */

#ifndef SHADER_H
#define SHADER_H
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
private:

	static const unsigned int INFO_LOG_SIZE = 512;
	GLuint id;

	/**
	 * Compiles a shader
	 * @param shaderType - The type of shader to compile
	 * @param shaderCode - The code of the shader to compile
	 * @return A shader ID
	 */
	GLuint compileShader(GLenum shaderType, const GLchar* shaderCode) const;

	/**
	 * Checks a shader or shader program for compile errors
	 * @param shader - The shader or shader program ID
	 * @param shaderType - The type of shader that the shader ID represents
	 */
	void checkCompileErrors(GLuint shader, GLenum shaderType = GL_NONE) const;

	/**
	 * Converts a shader type to a string representation
	 * @param shaderType - The shader type to convert
	 * @return A string representing a shader type
	 */
	std::string shaderTypeToString(GLenum shaderType) const;

public:
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(const Shader& other) = delete;
	Shader(Shader&& other) = delete;
	Shader& operator=(const Shader& other) = delete;
	Shader& operator=(Shader&& other) = delete;
	~Shader();

	/**
	 * Activates the shader program
	 */
	void use() const;

	/**
	 * Sets the value of a boolean uniform for the shader
	 * @param name - The name of the uniform
	 * @param value - The value to set the uniform to, true or false
	 */
	void setBool(const std::string& name, bool value) const;
	
	/**
	 * Sets the value of an integer uniform for the shader
	 * @param name - The name of the uniform
	 * @param value - The integer value to set the uniform to
	 */
	void setInt(const std::string& name, int value) const;
	
	/**
	 * Sets the value of a floating point uniform for the shader
	 * @param name - The name of the uniform
	 * @param value - The floating point value to set the uniform to
	 */
	void setFloat(const std::string& name, float value) const;
	
	/**
	 * Sets the value of a two-dimensional vector uniform for the shader
	 * @param name - The name of the uniform
	 * @param value - The two-dimensional vector to set the uniform to
	 */
	void setVec2(const std::string& name, const glm::vec2& value) const;
	
	/**
	 * Sets the value of a two-dimensional vector uniform for the shader
	 * @param name - The name of the uniform
	 * @param x - The x value of the vector
	 * @param y - The y value of the vector
	 */
	void setVec2(const std::string& name, float x, float y) const;
	
	/**
	 * Sets the value of a three-dimensional vector uniform for the shader
	 * @param name - The name of the uniform
	 * @param value - The three-dimensional vector to set the uniform to
	 */
	void setVec3(const std::string& name, const glm::vec3& value) const;
	
	/**
	 * Sets the value of a three-dimensional vector uniform for the shader
	 * @param name - The name of the uniform
	 * @param x - The x value of the vector
	 * @param y - The y value of the vector
	 * @param z - The z value of the vector
	 */
	void setVec3(const std::string& name, float x, float y, float z) const;
	
	/**
	 * Sets the value of a four-dimensional vector uniform for the shader
	 * @param name - The name of the uniform
	 * @param value - The four-dimensional vector to set the uniform to
	 */
	void setVec4(const std::string& name, const glm::vec4& value) const;
	
	/**
	 * Sets the value of a four-dimensional vector uniform for the shader
	 * @param name - The name of the uniform
	 * @param x - The x value of the vector
	 * @param y - The y value of the vector
	 * @param z - The z value of the vector
	 * @param w - The w value of the vector
	 */
	void setVec4(const std::string& name, float x, float y, float z, float w) const;
	
	/**
	 * Sets the value of a two-by-two matrix uniform for the shader
	 * @param name - The name of the uniform
	 * @param value - the two-by-two matrix to set the uniform to
	 */
	void setMat2(const std::string& name, const glm::mat2& mat) const;
	
	/**
	 * Sets the value of a three-by-three matrix uniform for the shader
	 * @param name - The name of the uniform
	 * @param value - the three-by-three matrix to set the uniform to
	 */
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	
	/**
	 * Sets the value of a four-by-four matrix uniform for the shader
	 * @param name - The name of the uniform
	 * @param value - the four-by-four matrix to set the uniform to
	 */
	void setMat4(const std::string& name, const glm::mat4& mat) const;
};

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
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
	} catch (std::ifstream::failure& e) {
		std::cout << "ERROR::SHADER::FILE_READ_UNSUCCESSFULLY\n" << e.what() << std::endl;
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
		if (!success) {
			glGetShaderInfoLog(shader, INFO_LOG_SIZE, nullptr, infoLog);
			std::cout << "ERROR::SHADER::" << shaderTypeToString(shaderType) << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	} else {
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(id, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
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

#endif