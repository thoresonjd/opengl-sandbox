/**
 * @file camera.h
 * @brief Flying free look camera
 * @date October 2023
 */

#ifndef CAMERA_H
#define CAMERA_H
#pragma once

#include <glm/glm.hpp>

/**
 * @class Camera - Camera that can look and move around freely
 */
class Camera {
private:

	static constexpr float DEFAULT_PITCH = 0.0f;
	static constexpr float DEFAULT_YAW = -90.0f;
	static constexpr float DEFAULT_MOVEMENT_SPEED = 5.0f;
	static constexpr float DEFAULT_MOUSE_SENSITIVITY = 0.1f;
	static constexpr float DEFAULT_FOV = 45.0f;
	glm::vec3 initialPosition;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldUp;
	float pitch;
	float yaw;
	float movementSpeed;
	float mouseSensitivity;
	float fieldOfView;
	
	/**
	 * Recomputes camera vectors: front, right, and up
	 */
	void updateCameraVectors();
public:

	/**
	 * Represents directions that the camera may move
	 */
	enum class Movement;
	
	Camera(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
		float pitch = DEFAULT_PITCH,
		float yaw = DEFAULT_YAW
	);
	Camera(
		float posX, float posY, float posZ,
		float worldUpX, float worldUpY, float worldUpZ,
		float pitch,
		float yaw
	);
	Camera(const Camera& other) = delete;
	Camera(Camera&& other) = delete;
	Camera& operator=(const Camera& other) = delete;
	Camera& operator=(Camera&& other) = delete;
	~Camera() = default;
	
	/**
	 * Processes keyboard input
	 * @param direction - The direction in which to move the camera
	 * @param deltaTime - The change in time to apply to movement velocity
	 */
	void processKeyboard(Movement direction, float deltaTime);

	/**
	 * Processes mouse movement
	 * @param offsetX - Amount in which the mouse has moved along the x-axis
	 * @param offsetY - Amount in which the mouse has moved along the y-axis
	 * @param constrainPitch - Determines if the pitch should be constrained within a safe range
	 */
	void processMouseMovement(float offsetX, float offsetY, bool constrainPitch = true);

	/**
	 * Processes mouse scroll
	 * @param offsetY - Amount in which the mouse has been scrolled
	 */
	void processMouseScroll(float offsetY);
	
	/**
	 * Computes the view matrix for the camera given it's position, front, and up vectors
	 */
	glm::mat4 getViewMatrix() const;

	/**
	 * Resets the camera to it's initial states
	 */
	void reset();

	/**
	 * Retrieves the camera's field of view (zoom)
	 */
	float getFOV() const;

	/**
	 *
	 */
	glm::vec3 getPosition() const;
};

#endif

enum class Camera::Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

Camera::Camera(
	glm::vec3 position,
	glm::vec3 worldUp,
	float pitch,
	float yaw
) : initialPosition(position),
	position(position),
	front(glm::vec3(0.0f, 0.0f, -1.0f)),
	worldUp(worldUp),
	pitch(pitch),
	yaw(yaw),
	movementSpeed(DEFAULT_MOVEMENT_SPEED),
	mouseSensitivity(DEFAULT_MOUSE_SENSITIVITY),
	fieldOfView(DEFAULT_FOV) {
	updateCameraVectors();
}

Camera::Camera(
	float posX, float posY, float posZ,
	float worldUpX, float worldUpY, float worldUpZ,
	float pitch,
	float yaw
) : initialPosition(position),
	position(glm::vec3(posX, posY, posZ)),
	front(glm::vec3(0.0f, 0.0f, -1.0f)),
	worldUp(glm::vec3(worldUpX, worldUpY, worldUpZ)),
	pitch(pitch),
	yaw(yaw),
	movementSpeed(DEFAULT_MOVEMENT_SPEED),
	mouseSensitivity(DEFAULT_MOUSE_SENSITIVITY),
	fieldOfView(DEFAULT_FOV) {
	updateCameraVectors();
}

void Camera::processKeyboard(Movement direction, float deltaTime) {
	float velocity = movementSpeed * deltaTime;
	switch (direction) {
		case Movement::FORWARD:
			position += front * velocity;
			break;
		case Movement::BACKWARD:
			position -= front * velocity;
			break;
		case Movement::LEFT:
			position -= right * velocity;
			break;
		case Movement::RIGHT:
			position += right * velocity;
			break;
	}
}

void Camera::processMouseMovement(float offsetX, float offsetY, bool constrainPitch) {
	offsetX *= mouseSensitivity;
	offsetY *= mouseSensitivity;
	yaw += offsetX;
	pitch += offsetY;
	if (constrainPitch) {
		if (pitch > 89.9f)
			pitch = 89.9f;
		else if (pitch < -89.9f)
			pitch = -89.9f;
	}
	updateCameraVectors();
}

void Camera::processMouseScroll(float offsetY) {
	fieldOfView -= offsetY;
	if (fieldOfView < 1.0f)
		fieldOfView = 1.0f;
	else if (fieldOfView > 45.0f)
		fieldOfView = 45.0f;
}

glm::mat4 Camera::getViewMatrix() const {
	return glm::lookAt(position, position + front, up);
}

float Camera::getFOV() const {
	return fieldOfView;
}

void Camera::reset() {
	position = initialPosition;
	pitch = DEFAULT_PITCH;
	yaw = DEFAULT_YAW;
	fieldOfView = DEFAULT_FOV;
	updateCameraVectors();
}

glm::vec3 Camera::getPosition() const {
	return position;
}

void Camera::updateCameraVectors() {
	glm::vec3 reverseDirection;
	reverseDirection.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	reverseDirection.y = sin(glm::radians(pitch));
	reverseDirection.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw)); 
	front = glm::normalize(reverseDirection);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}