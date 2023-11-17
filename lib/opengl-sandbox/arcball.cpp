/**
 * @file arcball.cpp
 * @brief Utilizes quaternions to rotate objects
 * @date October 2023
 * @see http://courses.cms.caltech.edu/cs171/assignments/hw3/hw3-notes/notes-hw3.html#NotesSection2
 */

#include <opengl-sandbox/arcball.h>

Arcball::Arcball(
	float radius,
	bool invertY
) : lastRotation(IDENTITY_QUATERNION),
	currentRotation(IDENTITY_QUATERNION),
	start(glm::vec2(0.0f)),
	end(glm::vec2(0.0f)),
	radius(radius),
	invertY(invertY) {
}

void Arcball::beginRotation(float posX, float posY) {
	beginRotation(glm::vec2(posX, posY));
}

void Arcball::beginRotation(glm::vec2 pos) {
	if (!invertY)
		pos.y *= -1.0f;
	start = pos;
	rotating = true;
}

void Arcball::rotate(float posX, float posY) {
	rotate(glm::vec2(posX, posY));
}

void Arcball::rotate(glm::vec2 pos) {
	if (!invertY)
		pos.y *= -1.0f;
	end = pos;
	currentRotation = computeRotationQuaternion(start, end);
}

void Arcball::endRotation() {
	lastRotation = currentRotation * lastRotation;
	currentRotation = IDENTITY_QUATERNION;
	rotating = false;
}

bool Arcball::isRotating() const {
	return rotating;
}

glm::mat4 Arcball::getRotationMatrix() const {
	return glm::mat4_cast(currentRotation * lastRotation);
};

glm::vec2 Arcball::screenToNDC(float posX, float posY, int width, int height) {
	return screenToNDC(glm::vec2(posX, posY), width, height);
}

glm::vec2 Arcball::screenToNDC(double posX, double posY, int width, int height) {
	float posXf = static_cast<float>(posX);
	float posYf = static_cast<float>(posY);
	return screenToNDC(glm::vec2(posXf, posYf), width, height);
}

glm::vec2 Arcball::screenToNDC(glm::vec2 pos, int width, int height) {
	float x = ((pos.x / static_cast<float>(width)) - 0.5f) * 2.0f;
	float y = ((pos.y / static_cast<float>(height)) - 0.5f) * 2.0f;
	return glm::vec2(x, y);
}

glm::quat Arcball::computeRotationQuaternion(glm::vec2 start, glm::vec2 end) const {
	glm::vec3 startPos = mapToSurface(start);
	glm::vec3 endPos = mapToSurface(end);
	float startDotEnd = glm::dot(startPos, endPos);
	float startMagnitude = glm::length(startPos);
	float endMagnitude = glm::length(endPos);
	float angle = acos(std::min(startDotEnd / (startMagnitude * endMagnitude), 1.0f));
	glm::vec3 axis = glm::normalize(glm::cross(startPos, endPos));
	return glm::normalize(glm::angleAxis(angle, axis));
}

glm::vec3 Arcball::mapToSurface(glm::vec2 pos) const {
	float rSquared = radius * radius;
	float xSquared = pos.x * pos.x;
	float ySquared = pos.y * pos.y;
	float z = 0;
	if (xSquared + ySquared <= rSquared)
		z = sqrt(rSquared - xSquared - ySquared);
	return glm::vec3(pos, z);
}