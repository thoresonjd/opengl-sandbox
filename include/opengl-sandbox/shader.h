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
#include <opengl-sandbox/logger.h>

/**
 * @class Shader - Represents shader program functionality
 */
class Shader {
private:

	static const unsigned int INFO_LOG_SIZE = 512;
	GLuint id;
	Logger* logger = nullptr;

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

	/**
	 * Default constructor
	 * @param vertexPath - The path of the vertex shader file
	 * @param fragmentPath - The path of the fragment shader file
	 * @param logger - A pointer to a logger to log errors
	 */
	Shader(const char* vertexPath, const char* fragmentPath, Logger* logger = nullptr);

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

#endif