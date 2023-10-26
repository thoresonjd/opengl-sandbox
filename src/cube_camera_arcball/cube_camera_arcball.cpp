/**
 * @file cube_camera_arcball.cpp
 * @brief Viewing a shaded, textured cube via an orbital camera
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
// image loading
#include <stb_image.h>
// C++
#include <string>
// Custom
#include <opengl-sandbox/shader.h>
#include <opengl-sandbox/camera_arcball.h>
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
 * Determines which image format to use given the number of channels
 * @param numChannels - The number of color components in an image
 * @return The corresponding OpenGL image format
 */
GLenum getImageFormat(int numChannels);

/**
 * Reads a texture into memory
 * @param path - The texture's file path
 * @param flipVertically - Determines whether to flip the texture along the y-axis
 * @return The ID of the loaded texture
 */
GLuint loadTexture(const char* path, bool flipVertically = true);

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

/**
 * Processes mouse buttons
 * @param window - A pointer to a GLFW window
 * @param button - A mouse button that is acted upon
 * @param action - An action that the mouse button is undergoing
 * @param mods - GLFW modifier bits
 */
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

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
CameraArcball camera(glm::vec3(0.0f, 0.0f, 7.0f));
bool firstMouse = true;
float lastY = windowHeight / 2.0f;
// logger
Logger logger(Logger::Output::CONSOLE);
// texture
const char* TEX = "assets/textures/tuxwalkinginrain.jpg";
// shader
const char* CUBE_VERT_SHADER = "src/cube_camera_arcball/cube.vs";
const char* CUBE_FRAG_SHADER = "src/cube_camera_arcball/cube.fs";
const char* LIGHT_VERT_SHADER = "src/cube_camera_arcball/light.vs";
const char* LIGHT_FRAG_SHADER = "src/cube_camera_arcball/light.fs";
// lighting/shading
const float LIGHT_SCALAR = 0.25f;
const float LIGHT_MOVEMENT_SPEED = 4.0f;
glm::vec3 lightColor(1.0f);
glm::vec3 lightPos(2.0f);
bool useBlinnPhongShading = true;
bool useBlinnPhongShadingKeyPressed = false;

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
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	// initialize GLAD
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		terminate(EXIT_FAILURE, "Failed to initialize GLAD");
	
	// log specs
	logger.log("GL vendor:", false, ' ');
	logger.log(glGetString(GL_VENDOR), false);
	logger.log("GL renderer:", false, ' ');
	logger.log(glGetString(GL_RENDERER), false);
	logger.log("GL version:", false, ' ');
	logger.log(glGetString(GL_VERSION), false);
	logger.log("GLSL version:", false, ' ');
	logger.log(glGetString(GL_SHADING_LANGUAGE_VERSION), false);
	logger.log("", false);

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

	// load texture
	GLuint texture = loadTexture(TEX);
	cubeShader.use();
	cubeShader.setInt("texture", 0);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processKeyboardInput(window);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// coordinate space transformations
		const float FRUSTUM_NEAR = 0.01f;
		const float FRUSTUM_FAR = 100.0f;
		glm::mat4 model;
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(camera.getFOV(), aspectRatio, FRUSTUM_NEAR, FRUSTUM_FAR);

		// render light source
		if (useBlinnPhongShading) {
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			model = glm::mat4(1.0f);
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(LIGHT_SCALAR));
			lightShader.use();
			lightShader.setVec3("lightColor", lightColor);
			lightShader.setMat4("model", model);
			lightShader.setMat4("view", view);
			lightShader.setMat4("projection", projection);
			cube.draw(lightShader);
		}

		// render cube
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
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
		cubeShader.setBool("useBlinnPhongShading", useBlinnPhongShading);
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

GLenum getImageFormat(int numChannels) {
	switch (numChannels) {
		case 1: return GL_RED;
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
		default: return GL_NONE;
	}
}

GLuint loadTexture(const char* path, bool flipVertically) {
	GLuint texture;
	glGenTextures(1, &texture);
	int width, height, numChannels;
	stbi_set_flip_vertically_on_load(flipVertically);
	stbi_uc* data = stbi_load(path, &width, &height, &numChannels, 0);
	GLenum format = getImageFormat(numChannels);
	if (data && format) {
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	} else
		logger.log("ERROR::TEXTURE::FAILED_TO_LOAD\n" + std::string{path});
	stbi_image_free(data);
	return texture;
}

void processKeyboardInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !useBlinnPhongShadingKeyPressed) {
		useBlinnPhongShading = !useBlinnPhongShading;
		useBlinnPhongShadingKeyPressed = true;
		logger.log("Blinn-Phong shading " + std::string{useBlinnPhongShading ? "enabled" : "disabled"});
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
		useBlinnPhongShadingKeyPressed = false;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		lightPos.z -= LIGHT_MOVEMENT_SPEED * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		lightPos.z += LIGHT_MOVEMENT_SPEED * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		lightPos.x -= LIGHT_MOVEMENT_SPEED * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		lightPos.x += LIGHT_MOVEMENT_SPEED * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		lightPos.y += LIGHT_MOVEMENT_SPEED * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		lightPos.y -= LIGHT_MOVEMENT_SPEED * deltaTime;
}

void mouseMovementCallback(GLFWwindow* window, double posX, double posY) {
	float positionX = static_cast<float>(posX);
	float positionY = static_cast<float>(posY);
	glm::vec2 pos = camera.screenToNDC(positionX, positionY, windowWidth, windowHeight);
	if (camera.isRotating())
		camera.rotate(pos);
	else if (camera.isTranslating()) {
		if (firstMouse) {
			lastY = pos.y;
			firstMouse = false;
		}
		float offsetY = lastY - pos.y; // reversed since y-coordinates range from top to bottom
		lastY = pos.y;
		camera.translate(offsetY);
	}
}

void mouseScrollCallback(GLFWwindow* window, double offsetX, double offsetY) {
	camera.adjustFOV(static_cast<float>(offsetY / 10.0f));
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double posX, posY;
		glfwGetCursorPos(window, &posX, &posY);
		glm::vec2 pos = camera.screenToNDC(posX, posY, windowWidth, windowHeight);
		camera.beginRotation(pos);
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		camera.endRotation();
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		camera.beginTranslation();
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		camera.endTranslation();
		firstMouse = true;
	}
}