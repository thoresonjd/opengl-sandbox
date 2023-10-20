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

// cube attributes
// ==================================================

/*
* cube vertices numbered:
*
*	      6 ---------------- 7
*	      _/|            _/|
*	    _/  |          _/  |
*	   /    |         /    |
*	3 ---------------- 2   |
*	  |     |        |     |
*	  |     |        |     |
*	  |     |        |     |
*	  |   5 ---------|------ 4
*	  |   _/         |   _/
*	  | _/           | _/
*	  |/             |/
*	0 ---------------- 1
*
* coordinate space:
* 
*	          +y
*	           |	
*	           |       _ -z
*	           |     _/
*	           |   _/
*	           | _/
*	-x ________|/_________ +x
*	         _/|	
*	       _/  |
*	     _/    |
*	+z _/      |
*	           |
*	           |
*	          -y
*/

std::vector<glm::vec3> Cube::vertices = {
	// front			   // array index - cube vertex
	{-1.0f, -1.0f, 1.0f},  // 0 - 0
	{1.0f, -1.0f, 1.0f},   // 1 - 1
	{1.0f, 1.0f, 1.0f},    // 2 - 2
	{-1.0f, 1.0f, 1.0f},   // 3 - 3
	// back				   
	{1.0f, -1.0f, -1.0f},  // 4 - 4
	{-1.0f, -1.0f, -1.0f}, // 5 - 5
	{-1.0f, 1.0f, -1.0f},  // 6 - 6
	{1.0f, 1.0f, -1.0f},   // 7 - 7
	// left
	{-1.0f, -1.0f, -1.0f}, // 8 - 5
	{-1.0f, -1.0f, 1.0f},  // 9 - 0
	{-1.0f, 1.0f, 1.0f},   // 10 - 3
	{-1.0f, 1.0f, -1.0f},  // 11 - 6
	// right
	{1.0f, -1.0f, 1.0f},   // 12 - 1
	{1.0f, -1.0f, -1.0f},  // 13 - 4
	{1.0f, 1.0f, -1.0f},   // 14 - 7
	{1.0f, 1.0f, 1.0f},    // 15 - 2
	// top
	{-1.0f, 1.0f, 1.0f},   // 16 - 3
	{1.0f, 1.0f, 1.0f},    // 17 - 2
	{1.0f, 1.0f, -1.0f},   // 18 - 7
	{-1.0f, 1.0f, -1.0f},  // 19 - 6
	// bottom
	{-1.0f, -1.0f, -1.0f}, // 20 - 5
	{1.0f, -1.0f, -1.0f},  // 21 - 4
	{1.0f, -1.0f, 1.0f},   // 22 - 1
	{-1.0f, -1.0f, 1.0f}   // 23 - 0
};

std::vector<glm::vec3> Cube::normals = {
	// front
	{0.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 1.0f},
	// back
	{0.0f, 0.0f, -1.0f},
	{0.0f, 0.0f, -1.0f},
	{0.0f, 0.0f, -1.0f},
	{0.0f, 0.0f, -1.0f},
	// left
	{-1.0f, 0.0f, 0.0f},
	{-1.0f, 0.0f, 0.0f},
	{-1.0f, 0.0f, 0.0f},
	{-1.0f, 0.0f, 0.0f},
	// right
	{1.0f, 0.0f, 0.0f},
	{1.0f, 0.0f, 0.0f},
	{1.0f, 0.0f, 0.0f},
	{1.0f, 0.0f, 0.0f},
	// top
	{0.0f, 1.0f, 0.0f},
	{0.0f, 1.0f, 0.0f},
	{0.0f, 1.0f, 0.0f},
	{0.0f, 1.0f, 0.0f},
	// bottom
	{0.0f, -1.0f, 0.0f},
	{0.0f, -1.0f, 0.0f},
	{0.0f, -1.0f, 0.0f},
	{0.0f, -1.0f, 0.0f}
};

std::vector<glm::vec2> Cube::textureCoordinates = {
	// front
	{0, 0},
	{1, 0},
	{1, 1},
	{0, 1},
	// back
	{0, 0},
	{1, 0},
	{1, 1},
	{0, 1},
	// left
	{0, 0},
	{1, 0},
	{1, 1},
	{0, 1},
	// right
	{0, 0},
	{1, 0},
	{1, 1},
	{0, 1},
	// top
	{0, 0},
	{1, 0},
	{1, 1},
	{0, 1},
	// bottom
	{0, 0},
	{1, 0},
	{1, 1},
	{0, 1}
};

std::vector<glm::vec4> Cube::colors = {
	// front
	{ 0.0f, 0.0f, 0.0f, 1.0f }, // black
	{ 1.0f, 0.0f, 0.0f, 1.0f }, // red
	{ 0.0f, 1.0f, 0.0f, 1.0f }, // green
	{ 0.0f, 0.0f, 1.0f, 1.0f }, // blue
	// back
	{ 1.0f, 1.0f, 0.0f, 1.0f }, // yellow
	{ 1.0f, 0.0f, 1.0f, 1.0f }, // magenta
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // white
	{ 0.0f, 1.0f, 1.0f, 1.0f }, // cyan
	// left
	{ 1.0f, 0.0f, 1.0f, 1.0f }, // magenta
	{ 0.0f, 0.0f, 0.0f, 1.0f }, // black
	{ 0.0f, 0.0f, 1.0f, 1.0f }, // blue
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // white
	// right
	{ 1.0f, 0.0f, 0.0f, 1.0f }, // red
	{ 1.0f, 1.0f, 0.0f, 1.0f }, // yellow
	{ 0.0f, 1.0f, 1.0f, 1.0f }, // cyan
	{ 0.0f, 1.0f, 0.0f, 1.0f }, // green
	// top
	{ 0.0f, 0.0f, 1.0f, 1.0f }, // blue
	{ 0.0f, 1.0f, 0.0f, 1.0f }, // green
	{ 0.0f, 1.0f, 1.0f, 1.0f }, // cyan
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // white
	// bottom
	{ 1.0f, 0.0f, 1.0f, 1.0f }, // magenta
	{ 1.0f, 1.0f, 0.0f, 1.0f }, // yellow
	{ 1.0f, 0.0f, 0.0f, 1.0f }, // red
	{ 0.0f, 0.0f, 0.0f, 1.0f }  // black
};

std::vector<glm::uvec3> Cube::indices = {
	// front
	{0, 1, 2},
	{0, 2, 3},
	// back
	{4, 5, 6},
	{4, 6, 7},
	// left
	{8, 9, 10},
	{8, 10, 11},
	// right
	{12, 13, 14},
	{12, 14, 15},
	// top
	{16, 17, 18},
	{16, 18, 19},
	// bottom
	{20, 21, 22},
	{20, 22, 23}
};

// methods
// ==================================================

Cube::Cube() {
	generateBuffers();
}

Cube::~Cube() {
	deleteBuffers();
}

void Cube::draw(const Shader& shader) const {
	shader.use();
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Cube::generateBuffers() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	std::size_t verticesSize = vertices.size() * sizeof(glm::vec3);
	std::size_t normalsSize = normals.size() * sizeof(glm::vec3);
	std::size_t texCoordsSize = textureCoordinates.size() * sizeof(glm::vec2);
	std::size_t colorsSize = colors.size() * sizeof(glm::vec4);
	std::size_t indicesSize = indices.size() * sizeof(glm::uvec3);
	glBufferData(GL_ARRAY_BUFFER, verticesSize + normalsSize + texCoordsSize + colorsSize, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, verticesSize, &vertices.front());
	glBufferSubData(GL_ARRAY_BUFFER, verticesSize, normalsSize, &normals.front());
	glBufferSubData(GL_ARRAY_BUFFER, verticesSize + normalsSize, texCoordsSize, &textureCoordinates.front());
	glBufferSubData(GL_ARRAY_BUFFER, verticesSize + normalsSize + texCoordsSize, colorsSize, &colors.front());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void*>(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void*>(verticesSize));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(verticesSize + normalsSize));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), reinterpret_cast<void*>(verticesSize + normalsSize + texCoordsSize));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, &indices.front(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Cube::deleteBuffers() const {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}