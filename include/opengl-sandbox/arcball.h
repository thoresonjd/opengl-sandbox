/**
 * @file arcball.h
 * @brief Utilizes quaternions to rotate objects
 * @date October 2023
 * @see http://courses.cms.caltech.edu/cs171/assignments/hw3/hw3-notes/notes-hw3.html#NotesSection2
 */

#ifndef ARCBALL_H
#define ARCBALL_H
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cmath>

/**
 * @class Arcball - Represents an arcball and corresponding quaternion rotations
 */
class Arcball {
private:
	static constexpr glm::quat IDENTITY_QUATERNION = glm::quat(1.0f, glm::vec3(0.0f));
	static constexpr float DEFAULT_RADIUS = 1.0f;
	glm::quat lastRotation;
	glm::quat currentRotation;
	glm::vec2 start;
	glm::vec2 end;
	float radius;
	bool invertY;
	bool isActive = false;

	/**
	 * Computes a rotation quaternion given a start point and an end point
	 * @param start - The starting position of a quaternion rotation
	 * @param end - The ending position of a quaternion rotation
	 * @return A quaternion representing a rotation
	 */
	glm::quat computeRotationQuaternion(glm::vec2 start, glm::vec2 end);

	/**
	 * Maps a cursor position to the surface of the arcball by computing the z-coordinate
	 * @param pos - A two-dimensional cursor position in NDC
	 * @return A three-dimensional position on the arcball surface
	 */
	glm::vec3 mapToSurface(glm::vec2 pos) const;

public:
	/**
	 * Default constructor
	 * @param radius - The arcball radius
	 * @param invertY - Determines if the y-coordinate should be inverted when performing a rotation
	 */
	Arcball(float radius = DEFAULT_RADIUS, bool invertY = false);
	Arcball(const Arcball& other) = delete;
	Arcball(Arcball&& other) = delete;
	Arcball& operator=(const Arcball& other) = delete;
	Arcball& operator=(Arcball&& other) = delete;
	~Arcball() = default;

	/**
	 * Starts the quarternion rotation by setting the starting position
	 * @param posX - The x-coordinate of a cursor position
	 * @param posY - The y-coordinate of a cursor rotation
	 */
	void down(float posX, float posY);

	/**
	 * Starts the quarternion rotation by setting the starting position
	 * @param pos - A two-dimensional cursor position
	 */
	void down(glm::vec2 pos);

	/**
	 * Applies the current position to the ongoing quaternion rotation
	 * @param posX - The x-coordinate of the current cursor position
	 * @param posY - The y-coordinate of the current cursor rotation
	 */
	void rotate(float posX, float posY);

	/**
	 * Applies the current position to the ongoing quaternion rotation
	 * @param pos - The current two-dimensional cursor position
	 */
	void rotate(glm::vec2 pos);

	/**
	 * Completes the quaternion arcball rotation
	 */
	void up();

	/**
	 * Returns the quaternion rotation as a four-by-four matrix
	 */
	glm::mat4 getRotation();

	/**
	 * Converts a cursor position from screen coordinates to normalized device coordinates
	 * @param posX - The x-coordinate of a cursor position
	 * @param posY - The y-coordinate of a cursor position
	 * @param width - The width of the screen
	 * @param height - The height of the screen
	 * @return The cursor position in normaized device coordinates
	 */
	static glm::vec2 screenToNDC(float posX, float posY, int width, int height);

	/**
	 * Converts a cursor position from screen coordinates to normalized device coordinates
	 * @param pos - A two-dimensional cursor position
	 * @param width - The width of the screen
	 * @param height - The height of the screen
	 * @return The cursor position in normaized device coordinates
	 */
	static glm::vec2 screenToNDC(glm::vec2 pos, int width, int height);
	
	/**
	 * Determines if the arcball is actively undergoing a rotation
	 */
	bool getActiveStatus() const;
};

#endif

Arcball::Arcball(
	float radius, bool invertY
) : lastRotation(IDENTITY_QUATERNION), 
	currentRotation(IDENTITY_QUATERNION),
	start(glm::vec2(0.0f)),
	end(glm::vec2(0.0f)),
	radius(radius),
	invertY(invertY) {
}

void Arcball::down(float posX, float posY) {
	down(glm::vec2(posX, posY));
}

void Arcball::down(glm::vec2 pos) {
	if (!invertY)
		pos.y *= -1.0f;
	start = pos;
	isActive = true;
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

void Arcball::up() {
	lastRotation = currentRotation* lastRotation;
	currentRotation = IDENTITY_QUATERNION;
	isActive = false;
}

glm::mat4 Arcball::getRotation() {
	return glm::mat4_cast(currentRotation* lastRotation);
};

glm::vec2 Arcball::screenToNDC(float posX, float posY, int width, int height) {
	return screenToNDC(glm::vec2(posX, posY), width, height);
}

glm::vec2 Arcball::screenToNDC(glm::vec2 pos, int width, int height) {
	float x = ((pos.x / static_cast<float>(width)) - 0.5f) * 2.0f;
	float y = ((pos.y / static_cast<float>(height)) - 0.5f) * 2.0f;
	return glm::vec2(x, y);
}

bool Arcball::getActiveStatus() const {
	return isActive;
}

glm::quat Arcball::computeRotationQuaternion(glm::vec2 start, glm::vec2 end) {
	glm::vec3 startPos = mapToSurface(start);
	glm::vec3 currentPos = mapToSurface(end);
	float startDotCurrent = glm::dot(startPos, currentPos);
	float startMagnitude = glm::length(startPos);
	float currentMagnitude = glm::length(currentPos);
	float angle = acos(std::min(startDotCurrent / (startMagnitude * currentMagnitude), 1.0f));
	glm::vec3 axis = glm::normalize(glm::cross(startPos, currentPos));
	return glm::normalize(glm::angleAxis(angle, axis));
}

glm::vec3 Arcball::mapToSurface(glm::vec2 pos) const {
	float xSquared = pos.x * pos.x;
	float ySquared = pos.y * pos.y;
	float z = 0;
	if (xSquared + ySquared <= radius)
		z = sqrt(radius - xSquared - ySquared);
	return glm::vec3(pos, z);
}