/**
 * @file cube.h
 * @brief Cube class for easy cube rendering
 * @date October 2023
 */

#ifndef CUBE_H
#define CUBE_H
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <opengl-sandbox/shader.h>
#include <vector>

/**
 * @class Cube - Represents a cube and it's basic rendering capabilities
 */
class Cube {
private:

	static std::vector<glm::vec3> vertices;
	static std::vector<glm::vec3> normals;
	static std::vector<glm::vec2> textureCoordinates;
	static std::vector<glm::vec4> colors;
	static std::vector<glm::uvec3> indices;
	GLuint vbo, vao, ebo;

	/**
	 * Generates all buffer data for the cube.
	 * Initializes vertex buffer object, vertex array object, and element buffer object.
	 */
	void generateBuffers();

	/**
	 * Deletes all buffer data for the cube
	 * Deletes vertex buffer object, vertex array object, and element buffer object.
	 */
	void deleteBuffers() const;

public:
	
	/**
	 * Default constructor
	 */
	Cube();

	Cube(const Cube& other) = delete;
	Cube(Cube&& other) = delete;
	Cube& operator=(const Cube& other) = delete;
	Cube& operator=(Cube&& other) = delete;
	~Cube();

	/**
	 * Displays the cube onto the screen
	 * @param shader - Reference to the shader to use when rendering the cube
	 */
	void draw(const Shader& shader) const;
};

#endif