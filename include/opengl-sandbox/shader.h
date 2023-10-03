/**
 * @file shader.h
 * @brief Shader class supporting vertex and fragment shaders
 * @date October 2023
 */

#ifndef SHADER_H
#define SHADER_H
#pragma once

#include <glad/glad.h>
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
	GLuint compileShader(GLenum shaderType, const char* shaderCode) const;

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

GLuint Shader::compileShader(GLenum shaderType, const char* shaderCode) const {
	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderCode, nullptr);
	glCompileShader(shader);
	checkCompileErrors(shader, shaderType);
	return shader;
}

void Shader::checkCompileErrors(GLuint shader, GLenum shaderType) const {
	int success;
	char infoLog[INFO_LOG_SIZE];
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