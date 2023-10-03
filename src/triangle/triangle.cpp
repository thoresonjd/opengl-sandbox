/**
 * @file triangle.cpp
 * @brief Rendering a triangle
 * @date October 2023
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <opengl-sandbox/shader.h>

// function prototypes
// ==================================================

/**
 * Stops the program
 * @param code - An exit code
 * @param message - A message detailing program termination
 */
void terminate(int code, std::string message);

/**
 * Handles framebuffer resizing for a given window
 * @param window - A pointer to a GLFW window
 * @param width - The new framebuffer width
 * @param height - The new framebuffer height
 */
void framebufferSizeCallback(GLFWwindow* window, GLsizei width, GLsizei height);

/**
 * Processes keyboard input
 * @param window - A pointer to a GLFW window
 */
void processKeyboardInput(GLFWwindow* window);

// settings
// ==================================================

const int OPENGL_VERSION_MAJOR = 3;
const int OPENGL_VERSION_MINOR = 3;
const char* WINDOW_NAME = "Triangle";
GLsizei windowWidth = 800;
GLsizei windowHeight = 600;
GLfloat aspectRatio = static_cast<GLfloat>(windowWidth) / static_cast<GLfloat>(windowHeight);

// function definitions
// ==================================================

int main(void) {
	// initialize GLFW
	if (!glfwInit())
		terminate(EXIT_FAILURE, "Failed to initialize GLFW");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	// create window
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, WINDOW_NAME, nullptr, nullptr);
	if (!window)
		terminate(EXIT_FAILURE, "Failed to create GLFW window");
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		terminate(EXIT_FAILURE, "Failed to initialize GLAD");

	// configure OpenGL
	glEnable(GL_MULTISAMPLE);

	// create triangle shader program
	Shader triangleShader("src/triangle/triangle.vs", "src/triangle/triangle.fs");

	// triangle attributes
	GLfloat vertices[][3] = {
		{-0.5f, -0.5f, 0.0f},
		{0.0f, 0.5f, 0.0f},
		{0.5f, -0.5f, 0.0f}
	};
	GLfloat colors[][4] = {
		{1.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f, 1.0f}
	};

	// triangle vertex buffer and vertex attribute objects
	GLuint triangleVBO, triangleVAO;
	glGenVertexArrays(1, &triangleVAO);
	glGenBuffers(1, &triangleVBO);
	glBindVertexArray(triangleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), &colors);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)sizeof(vertices));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		processKeyboardInput(window);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		triangleShader.use();
		glBindVertexArray(triangleVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// deallocate resources
	glDeleteVertexArrays(1, &triangleVAO);
	glDeleteBuffers(1, &triangleVBO);
	glfwTerminate();
	return EXIT_SUCCESS;
}

void terminate(int code, std::string message) {
	std::cout << message << std::endl;
	glfwTerminate();
	exit(code);
}

void framebufferSizeCallback(GLFWwindow* window, GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
	aspectRatio = static_cast<GLfloat>(windowWidth) / static_cast<GLfloat>(windowHeight);
}

void processKeyboardInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}