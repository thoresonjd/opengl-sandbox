/**
 * @file camera_arcball.cpp
 * @brief Orbital camera derived from an arcball
 * @date October 2023
 */

#include <opengl-sandbox/camera_arcball.h>

CameraArcball::CameraArcball(
	glm::vec3 position,
	glm::vec3 target,
	glm::vec3 worldUp
) : Arcball(),
	position(position),
	target(target),
	worldUp(worldUp),
	fieldOfView(DEFAULT_FOV) {
	updateCameraVectors();
}

CameraArcball::CameraArcball(
	float posX, float posY, float posZ,
	float targetX, float targetY, float targetZ,
	float worldUpX, float worldUpY, float worldUpZ
) : Arcball(),
	position(glm::vec3(posX, posY, posZ)),
	target(glm::vec3(targetX, targetY, targetZ)),
	worldUp(glm::vec3(worldUpX, worldUpY, worldUpZ)),
	fieldOfView(DEFAULT_FOV) {
	updateCameraVectors();
}

void CameraArcball::beginTranslation() {
	translating = true;
}

void CameraArcball::translate(float offset) {
	float velocity = -offset * TRANSLATION_SPEED;
	position += front * velocity;
}

void CameraArcball::endTranslation() {
	translating = false;
}

bool CameraArcball::isTranslating() const {
	return translating;
}

void CameraArcball::adjustFOV(float offset) {
	fieldOfView -= offset;
	if (fieldOfView < MIN_FOV)
		fieldOfView = MIN_FOV;
	else if (fieldOfView > MAX_FOV)
		fieldOfView = MAX_FOV;
}

glm::mat4 CameraArcball::getViewMatrix()  {
	return glm::lookAt(position, front, up) * getRotationMatrix();
}

float CameraArcball::getFOV() const {
	return fieldOfView;
}

glm::vec3 CameraArcball::getPosition() const {
	return position;
}

void CameraArcball::updateCameraVectors() {
	front = glm::normalize(position - target);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}