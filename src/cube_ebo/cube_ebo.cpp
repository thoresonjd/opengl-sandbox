/**
 * @file cube_ebo.cpp
 * @brief Rendering a cube via an element buffer object
 * @date October 2023
 */

// preprocessor directives
// ==================================================

// OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// C++
#include <string>
#include <iostream>
#include <cmath>
// Custom
#include <opengl-sandbox/shader.h>
#include <opengl-sandbox/camera.h>
#include <opengl-sandbox/logger.h>

// function prototypes
// ==================================================

/**
 * Stops the program
 * @param code - An exit code
 * @param message - A message detailing program termination
 */
void terminate(int exitCode, std::string message);

/**
 * Handles framebuffer resizing for a given window
 * @param window - A pointer to a GLFW window
 * @param width - The new framebuffer width
 * @param height - The new framebuffer height
 */
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

/**
 * Processes keyboard input
 * @param window - A pointer to a GLFW window
 */
void processKeyboardInput(GLFWwindow* window);

/**
 * Processes mouse movement
 * @param window - A pointer to a GLFW window
 * @param posX - The mouse's position along the x-axis
 * @param posY - The mouse's position along the y-axis
 */
void mouseMovementCallback(GLFWwindow* window, double posX, double posY);

/**
 * Processes mouse scroll
 * @param window - A pointer to a GLFW window
 * @param offsetX - Amount in which the mouse is scrolled horizontally
 * @param offsetY - Amount in which the mouse is scrolled vertically
 */
void mouseScrollCallback(GLFWwindow* window, double offsetX, double offsetY);

// settings
// ==================================================

// GLFW
const int OPENGL_VERSION_MAJOR = 3;
const int OPENGL_VERSION_MINOR = 3;
const char* WINDOW_NAME = "Cube";
GLsizei windowWidth = 800;
GLsizei windowHeight = 600;
float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = true;
float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;
// logger
Logger logger(Logger::Output::CONSOLE);
// shader
const char* CUBE_VERT_SHADER = "src/cube_ebo/cube.vs";
const char* CUBE_FRAG_SHADER = "src/cube_ebo/cube.fs";

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
	glfwSetCursorPosCallback(window, mouseMovementCallback);
	glfwSetScrollCallback(window, mouseScrollCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// initialize GLAD
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		terminate(EXIT_FAILURE, "Failed to initialize GLAD");

	// configure OpenGL
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// create shader program objects
	Shader cubeShader(CUBE_VERT_SHADER, CUBE_FRAG_SHADER, &logger);

	// cube attributes
	glm::vec3 vertices[] = {
		// front
		{ -1.0f, -1.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f },
		{ 1.0f, -1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		// back
		{ -1.0f, -1.0f, -1.0f },
		{ -1.0f, 1.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f },
		{ 1.0f, 1.0f, -1.0f }
	};
	glm::vec4 colors[] = {
		{ 1.0f, 0.0f, 0.0f, 1.0f }, // red
		{ 0.0f, 1.0f, 0.0f, 1.0f }, // green
		{ 0.0f, 0.0f, 1.0f, 1.0f }, // blue
		{ 1.0f, 1.0f, 1.0f, 1.0f }, // white
		{ 1.0f, 1.0f, 0.0f, 1.0f }, // yellow
		{ 0.0f, 1.0f, 1.0f, 1.0f }, // cyan
		{ 1.0f, 0.0f, 1.0f, 1.0f }, // magenta
		{ 0.0f, 0.0f, 0.0f, 1.0f }  // black
	};
	glm::uvec3 indices[] = {
		// front
		{ 0, 2, 1},
		{ 1, 2, 3},
		// left
		{ 0, 1, 4},
		{ 1, 5, 4},
		// right
		{ 2, 6, 3},
		{ 3, 6, 7},
		// back
		{ 4, 5, 6},
		{ 5, 7, 6},
		// top
		{ 1, 3, 5},
		{ 3, 7, 5},
		// bottom
		{ 0, 4, 2},
		{ 2, 4, 6}
	};

	// cube vertex buffer, vertex attribute, element buffer objects
	GLuint cubeVBO, cubeVAO, cubeEBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glGenBuffers(1, &cubeEBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), &colors);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(sizeof(vertices)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// keboard input
		processKeyboardInput(window);
		
		// set background color
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		// clear buffer bits to prevent information overlap between frames
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render cube
		glBindVertexArray(cubeVAO);
		cubeShader.use();
		glm::mat4 model(1.0f);
		glm::mat4 view = camera.getViewMatrix();
		// compute proper aspect ratio for multiple viewports
		GLsizei halfWindowWidth = windowWidth / 2;
		aspectRatio = static_cast<float>(halfWindowWidth) / static_cast<float>(windowHeight);
		glm::mat4 projection = glm::perspective(camera.getFOV(), aspectRatio, 0.1f, 100.0f);
		cubeShader.setMat4("model", model);
		cubeShader.setMat4("view", view);
		cubeShader.setMat4("projection", projection);
		// as normal
		glViewport(0, 0, halfWindowWidth, windowHeight);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		// as wireframe
		glViewport(halfWindowWidth, 0, halfWindowWidth, windowHeight);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// swap buffers and poll events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// deallocate resources
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &cubeEBO);
	logger.log("Program exited with status " + std::to_string(EXIT_SUCCESS));
	glfwTerminate();
	return EXIT_SUCCESS;
}

void terminate(int code, std::string message) {
	logger.log(message);
	logger.log("Program exited with status " + std::to_string(code));
	glfwTerminate();
	exit(code);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
	aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
}

void processKeyboardInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(Camera::Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(Camera::Movement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(Camera::Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(Camera::Movement::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		camera.reset();
}

void mouseMovementCallback(GLFWwindow* window, double posX, double posY) {
	float positionX = static_cast<float>(posX);
	float positionY = static_cast<float>(posY);
	if (firstMouse) {
		lastX = positionX;
		lastY = positionY;
		firstMouse = false;
	}
	float offsetX = positionX - lastX;
	float offsetY = lastY - positionY; // reversed since y-coordinates range from top to bottom
	lastX = positionX;
	lastY = positionY;
	camera.processMouseMovement(offsetX, offsetY);
}

void mouseScrollCallback(GLFWwindow* window, double offsetX, double offsetY) {
	camera.processMouseScroll(static_cast<float>(offsetY / 10.0));
}