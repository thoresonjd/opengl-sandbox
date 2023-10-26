/**
 * @file camera.cpp
 * @brief Flying free look camera
 * @date October 2023
 */

#include <opengl-sandbox/camera.h>

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
	fieldOfView(DEFAULT_FOV) {
	updateCameraVectors();
}

void Camera::move(Movement direction, float deltaTime) {
	float velocity = MOVEMENT_SPEED * deltaTime;
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

void Camera::look(float offsetX, float offsetY, bool constrainPitch) {
	offsetX *= LOOK_SENSITIVITY;
	offsetY *= LOOK_SENSITIVITY;
	yaw += offsetX;
	pitch += offsetY;
	if (constrainPitch) {
		if (pitch > MAX_PITCH)
			pitch = MAX_PITCH;
		else if (pitch < MIN_PITCH)
			pitch = MIN_PITCH;
	}
	updateCameraVectors();
}

void Camera::adjustFOV(float offset) {
	fieldOfView -= offset;
	if (fieldOfView < MIN_FOV)
		fieldOfView = MIN_FOV;
	else if (fieldOfView > MAX_FOV)
		fieldOfView = MAX_FOV;
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