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
// Custom
#include <opengl-sandbox/shader.h>
#include <opengl-sandbox/camera.h>
#include <opengl-sandbox/logger.h>
#include <opengl-sandbox/cube.h>

// function prototypes
// ==================================================

/**
 * Initializes GLFW
 */
void initGLFW();

/**
 * Creates a window object
 * @return A pointer to the GLFW window
 */
GLFWwindow* initWindow();

/**
 * Sets all window callback
 * @param window - A pointer to a GLFW window
 */
void initCallbacks(GLFWwindow* window);

/**
 * Initializes GLAD
 */
void initGLAD();

/**
 * Stops the program
 * @param code - An exit code
 * @param message - A message detailing program termination
 */
void terminate(int exitCode, std::string message);

/**
 * Prints program specifications to the logger
 */
void logSpecs();

/**
 * Handles framebuffer resizing for a given window
 * @param window - A pointer to a GLFW window
 * @param width - The new framebuffer width
 * @param height - The new framebuffer height
 */
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

/**
 * Processes per-frame time logic (change in time since previous frame)
 */
void processTime();

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
bool resetKeyPressed = false;
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
struct {
	float scalar = 0.25f;
	float movementSpeed = 4.0f;
	glm::vec3 position = glm::vec3(2.0f);
	glm::vec3 color = glm::vec3(1.0f);
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
} light;

// function definitions
// ==================================================

int main(void) {
	// initialize
	initGLFW();
	GLFWwindow* window = initWindow();
	initCallbacks(window);
	initGLAD();
	logSpecs();

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
		processTime();
		processKeyboardInput(window);

		// set color and clear buffer bits
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// coordinate space transformations
		const float FRUSTUM_NEAR = 0.01f;
		const float FRUSTUM_FAR = 100.0f;
		glm::mat4 model(1.0f);
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(camera.getFOV(), aspectRatio, FRUSTUM_NEAR, FRUSTUM_FAR);

		// render light source
		model = glm::translate(model, light.position);
		model = glm::scale(model, glm::vec3(light.scalar));
		lightShader.use();
		lightShader.setVec3("lightColor", light.color);
		lightShader.setMat4("model", model);
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);
		cube.draw(lightShader);

		// render cube
		model = glm::mat4(1.0f);
		cubeShader.use();
		cubeShader.setVec3("light.position", light.position);
		cubeShader.setVec3("light.ambient", light.color);
		cubeShader.setVec3("light.diffuse", light.color);
		cubeShader.setVec3("light.specular", light.color);
		cubeShader.setFloat("light.constant", light.constant);
		cubeShader.setFloat("light.linear", light.linear);
		cubeShader.setFloat("light.quadratic", light.quadratic);
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

void initGLFW() {
	if (!glfwInit())
		terminate(EXIT_FAILURE, "Failed to initialize GLFW");
}

GLFWwindow* initWindow() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, WINDOW_NAME, nullptr, nullptr);
	if (!window)
		terminate(EXIT_FAILURE, "Failed to create GLFW window");
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return window;
}

void initCallbacks(GLFWwindow* window) {
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseMovementCallback);
	glfwSetScrollCallback(window, mouseScrollCallback);
}

void initGLAD() {
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		terminate(EXIT_FAILURE, "Failed to initialize GLAD");
}

void terminate(int code, std::string message) {
	logger.log(message);
	logger.log("Program exited with status " + std::to_string(code));
	glfwTerminate();
	exit(code);
}

void logSpecs() {
	logger.log("GL vendor:", false, ' ');
	logger.log(glGetString(GL_VENDOR), false);
	logger.log("GL renderer:", false, ' ');
	logger.log(glGetString(GL_RENDERER), false);
	logger.log("GL version:", false, ' ');
	logger.log(glGetString(GL_VERSION), false);
	logger.log("GLSL version:", false, ' ');
	logger.log(glGetString(GL_SHADING_LANGUAGE_VERSION), false);
	logger.log("", false);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
	aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
}

void processTime() {
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void processKeyboardInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.move(Camera::Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.move(Camera::Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.move(Camera::Movement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.move(Camera::Movement::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.move(Camera::Movement::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !resetKeyPressed) {
		camera.reset();
		resetKeyPressed = true;
		logger.log("Camera reset to default orientation");
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
		resetKeyPressed = false;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		light.position.z -= light.movementSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		light.position.z += light.movementSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		light.position.x -= light.movementSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		light.position.x += light.movementSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		light.position.y += light.movementSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		light.position.y -= light.movementSpeed * deltaTime;
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
	camera.look(offsetX, offsetY);
}

void mouseScrollCallback(GLFWwindow* window, double offsetX, double offsetY) {
	camera.adjustFOV(static_cast<float>(offsetY / 10.0f));
}