/**
 * @file cube_shaded.cpp
 * @brief Rendering a cube with a point light using Blinn-Phong shading
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
#include <vector>
// Custom
#include <opengl-sandbox/shader.h>
#include <opengl-sandbox/camera.h>
#include <opengl-sandbox/logger.h>
#include <opengl-sandbox/cube.h>

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
const char* CUBE_VERT_SHADER = "src/cube_shaded/cube.vs";
const char* CUBE_FRAG_SHADER = "src/cube_shaded/cube.fs";
const char* LIGHT_VERT_SHADER = "src/cube_shaded/light.vs";
const char* LIGHT_FRAG_SHADER = "src/cube_shaded/light.fs";
// lighting
const float LIGHT_SCALAR = 0.25f;
const float LIGHT_MOVEMENT_SPEED = 4.0f;
glm::vec3 lightColor(1.0f);
glm::vec3 lightPos(2.0f);

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
	Shader lightShader(LIGHT_VERT_SHADER, LIGHT_FRAG_SHADER, &logger);

	// cube object
	Cube cube;

	// render loop
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processKeyboardInput(window);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// coordinate space transformations
		const float FRUSTUM_NEAR = 0.01f;
		const float FRUSTUM_FAR = 100.0f;
		glm::mat4 model(1.0f);
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(camera.getFOV(), aspectRatio, FRUSTUM_NEAR, FRUSTUM_FAR);

		// render light source
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(LIGHT_SCALAR));
		lightShader.use();
		lightShader.setVec3("lightColor", lightColor);
		lightShader.setMat4("model", model);
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);
		cube.draw(lightShader);

		// render cube
		model = glm::mat4(1.0f);
		cubeShader.use();
		cubeShader.setVec3("light.position", lightPos);
		cubeShader.setVec3("light.ambient", lightColor);
		cubeShader.setVec3("light.diffuse", lightColor);
		cubeShader.setVec3("light.specular", lightColor);
		cubeShader.setFloat("light.constant", 1.0f);
		cubeShader.setFloat("light.linear", 0.09f);
		cubeShader.setFloat("light.quadratic", 0.032f);
		cubeShader.setVec3("viewPos", camera.getPosition());
		cubeShader.setMat4("model", model);
		cubeShader.setMat4("view", view);
		cubeShader.setMat4("projection", projection);
		cube.draw(cubeShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(Camera::Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(Camera::Movement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(Camera::Movement::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		camera.reset();
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		lightPos.z -= LIGHT_MOVEMENT_SPEED * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		lightPos.z += LIGHT_MOVEMENT_SPEED * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		lightPos.x -= LIGHT_MOVEMENT_SPEED * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		lightPos.x += LIGHT_MOVEMENT_SPEED * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		lightPos.y += LIGHT_MOVEMENT_SPEED * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		lightPos.y -= LIGHT_MOVEMENT_SPEED * deltaTime;
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