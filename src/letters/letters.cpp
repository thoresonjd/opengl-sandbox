/**
 * @file letters.cpp
 * @brief Rendering letters: JDTII
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
#include <vector>
// Custom
#include <opengl-sandbox/shader.h>
#include <opengl-sandbox/camera_arcball.h>
#include <opengl-sandbox/logger.h>

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
const char* WINDOW_NAME = "Letters";
GLsizei windowWidth = 800;
GLsizei windowHeight = 600;
float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
// camera
CameraArcball camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouseButtonRight = true;
float lastMouseY = windowHeight / 2.0f;
// logger
Logger logger(Logger::Output::CONSOLE);
// shader
const char* LETTER_VERT_SHADER = "src/letters/letter.vs";
const char* LETTER_FRAG_SHADER = "src/letters/letter.fs";

// function definitions
// ==================================================

int main(void) {
	// initialize
	initGLFW();
	GLFWwindow* window = initWindow();
	initCallbacks(window);
	initGLAD();
	logSpecs();

	// configure OpenGL capabilities
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	// create shader program objects
	Shader letterShader(LETTER_VERT_SHADER, LETTER_FRAG_SHADER, &logger);

	// letter attributes
	std::vector<glm::vec2> jVertices = {
		{.125f, .5f}, {-.125f, .5f}, {.875f, .5f},
		{-.875f, .5f}, {.875f, .75f}, {-.875f, .75f},
		{-.125f, -.5f}, {.125f, -.75f}, {-.875f, -.5f},
		{-.875f, -.75f}
	};
	std::vector<glm::vec2> dVertices = {
		{-.875f, .75f}, {-.875f, -.75f}, {-.625f, .5f},
		{-.625f, -.5f}, {-.375f, .75f}, {-.375f, -.75f},
		{.375f, 0}, {.875f, 0}
	};
	std::vector<glm::vec2> tVertices = {
		{.125f, .5f}, {-.125f, .5f}, {.875f, .5f},
		{-.875f, .5f}, {.875f, .75f}, {-.875f, .75f},
		{-.125f, -.5f}, {.125f, -.75f},
	};
	std::vector<glm::vec2> iiVertices = {
		{-.875f, .75f}, {-.875f, .5f}, {-.875f, -.5f},
		{-.875f, -.75f}, {-.375f, .5f}, {-.375f, -.5f},
		{-.125f, .5f}, {-.125f, -.5f}, {.125f, .5f},
		{.125f, -.5f}, {.375f, .5f}, {.375f, -.5f},
		{.875f, .75f}, {.875f, .5f}, {.875f, -.5f},
		{.875f, -.75f},
	};
	std::vector<glm::vec3> jColors = {
		{1, 0, 0}, {1, 0, 0}, {1, 0, 1},
		{0, 0, 0}, {1, 0, 1}, {0, 0, 0},
		{1, 1, 0}, {1, 1, 0}, {0, 1, 1},
		{0, 1, 1}
	};
	std::vector<glm::vec3> dColors = {
		{0, 0, 0}, {0, 1, 1}, {1, 0, 0},
		{0, 1, 1}, {1, 0, 1}, {0, 1, 0},
		{1, 1, 0}, {1, 1, 0}
	};
	std::vector<glm::vec3> tColors = {
		{1, 0, 0}, {1, 0, 0}, {1, 0, 1},
		{0, 0, 0}, {1, 0, 1}, {0, 0, 0},
		{1, 1, 0}, {1, 1, 0}
	};
	std::vector<glm::vec3> iiColors = {
		{0, 0, 0}, {0, 0, 0}, {0, 1, 1},
		{0, 1, 1}, {1, 0, 0}, {0, 1, 1},
		{1, 0, 0}, {1, 1, 0}, {0, 1, 1},
		{1, 1, 0}, {0, 1, 1}, {1, 1, 0},
		{1, 0, 1}, {1, 0, 1}, {1, 1, 0},
		{1, 1, 0}
	};
	std::vector<glm::uvec3> jIndices = {
		{0, 1, 4}, {0, 2, 4}, {0, 1, 7}, {1, 3, 5},
		{1, 4, 5}, {1, 6, 7}, {6, 7, 9}, {6, 8, 9}
	};
	std::vector<glm::uvec3> dIndices = {
		{10, 11, 13}, {10, 12, 13}, {10, 12, 14}, {11, 13, 15},
		{12, 14, 16}, {13, 15, 16}, {14, 16, 17}, {15, 16, 17}
	};
	std::vector<glm::uvec3> tIndices = {
		{18, 19, 22}, {18, 20, 22}, {18, 19, 25}, {19, 21, 23},
		{19, 22, 23}, {19, 24, 25}
	};
	std::vector<glm::uvec3> iiIndices = {
		{26, 27, 30}, {26, 30, 32}, {26, 32, 38}, {28, 29, 31},
		{29, 31, 33}, {29, 33, 35}, {29, 35, 41}, {30, 31, 33},
		{30, 32, 33}, {32, 34, 38}, {34, 35, 36}, {34, 36, 38},
		{35, 36, 37}, {35, 37, 41}, {36, 38, 39}, {37, 40, 41}
	};
	std::size_t jElementsSize = jIndices.size() * sizeof(glm::uvec3) / sizeof(GLuint);
	std::size_t dElementsSize = dIndices.size() * sizeof(glm::uvec3) / sizeof(GLuint);
	std::size_t tElementsSize = tIndices.size() * sizeof(glm::uvec3) / sizeof(GLuint);
	std::size_t iiElementsSize = iiIndices.size() * sizeof(glm::uvec3) / sizeof(GLuint);

	// letter vertex buffer and vertex array objects
	GLuint lettersVBO, lettersVAO;
	glGenVertexArrays(1, &lettersVAO);
	glGenBuffers(1, &lettersVBO);
	glBindVertexArray(lettersVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lettersVBO);
	std::size_t jVerticesSize = jVertices.size() * sizeof(glm::vec2);
	std::size_t dVerticesSize = dVertices.size() * sizeof(glm::vec2);
	std::size_t tVerticesSize = tVertices.size() * sizeof(glm::vec2);
	std::size_t iiVerticesSize = iiVertices.size() * sizeof(glm::vec2);
	std::size_t verticesSize = jVerticesSize + dVerticesSize + tVerticesSize + iiVerticesSize;
	std::size_t jColorsSize = jColors.size() * sizeof(glm::vec3);
	std::size_t dColorsSize = dColors.size() * sizeof(glm::vec3);
	std::size_t tColorsSize = tColors.size() * sizeof(glm::vec3);
	std::size_t iiColorsSize = iiColors.size() * sizeof(glm::vec3);
	std::size_t colorsSize = jColorsSize + dColorsSize + tColorsSize + iiColorsSize;
	glBufferData(GL_ARRAY_BUFFER, verticesSize + colorsSize, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, jVerticesSize, &jVertices.front());
	glBufferSubData(GL_ARRAY_BUFFER, jVerticesSize, dVerticesSize, &dVertices.front());
	glBufferSubData(GL_ARRAY_BUFFER, jVerticesSize + dVerticesSize, tVerticesSize, &tVertices.front());
	glBufferSubData(GL_ARRAY_BUFFER, jVerticesSize + dVerticesSize + tVerticesSize, iiVerticesSize, &iiVertices.front());
	glBufferSubData(GL_ARRAY_BUFFER, verticesSize, jColorsSize, &jColors.front());
	glBufferSubData(GL_ARRAY_BUFFER, verticesSize + jColorsSize, dColorsSize, &dColors.front());
	glBufferSubData(GL_ARRAY_BUFFER, verticesSize + jColorsSize + dColorsSize, tColorsSize, &tColors.front());
	glBufferSubData(GL_ARRAY_BUFFER, verticesSize + jColorsSize + dColorsSize + tColorsSize, iiColorsSize, &iiColors.front());
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), reinterpret_cast<void*>(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void*>(verticesSize));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		processTime();
		processKeyboardInput(window);

		// set color and clear buffer bits
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// coordinate space transformations
		const float FRUSTUM_NEAR = 0.01f;
		const float FRUSTUM_FAR = 100.0f;
		glm::mat4 model;
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(camera.getFOV(), aspectRatio, FRUSTUM_NEAR, FRUSTUM_FAR);

		// render letters
		glBindVertexArray(lettersVAO);
		letterShader.use();
		letterShader.setMat4("view", view);
		letterShader.setMat4("projection", projection);
		float scalar = 0.5f;
		// J
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(scalar));
		letterShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, static_cast<GLuint>(jElementsSize), GL_UNSIGNED_INT, &jIndices.front());
		// D
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scalar));
		letterShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, static_cast<GLuint>(dElementsSize), GL_UNSIGNED_INT, &dIndices.front());
		// T
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scalar));
		letterShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, static_cast<GLuint>(tElementsSize), GL_UNSIGNED_INT, &tIndices.front());
		// II
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scalar));
		letterShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, static_cast<GLuint>(iiElementsSize), GL_UNSIGNED_INT, &iiIndices.front());
		glBindVertexArray(0);

		// swap buffers and poll events
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
	return window;
}

void initCallbacks(GLFWwindow* window) {
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseMovementCallback);
	glfwSetScrollCallback(window, mouseScrollCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
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
}

void mouseMovementCallback(GLFWwindow* window, double posX, double posY) {
	float positionX = static_cast<float>(posX);
	float positionY = static_cast<float>(posY);
	glm::vec2 pos = camera.screenToNDC(positionX, positionY, windowWidth, windowHeight);
	if (camera.isRotating())
		camera.rotate(pos);
	else if (camera.isTranslating()) {
		if (firstMouseButtonRight) {
			lastMouseY = pos.y;
			firstMouseButtonRight = false;
		}
		float offsetY = lastMouseY - pos.y; // reversed since y-coordinates range from top to bottom
		lastMouseY = pos.y;
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
		firstMouseButtonRight = true;
	}
}