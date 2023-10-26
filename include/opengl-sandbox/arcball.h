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
#include <algorithm>

/**
 * @class Arcball - Represents an arcball and corresponding quaternion rotations.
 * Uses the entire screen as the frame of reference via NDC.
 */
class Arcball {
private:
	static constexpr float DEFAULT_RADIUS = 1.0f;
	static constexpr glm::quat IDENTITY_QUATERNION = glm::quat(1.0f, glm::vec3(0.0f));
	float radius;
	glm::vec2 start;
	glm::vec2 end;
	glm::quat lastRotation;
	glm::quat currentRotation;
	bool invertY;
	bool isActive = false;

	/**
	 * Computes a rotation quaternion given a start point and an end point
	 * @param start - The starting position of a quaternion rotation
	 * @param end - The ending position of a quaternion rotation
	 * @return A quaternion representing a rotation
	 */
	glm::quat computeRotationQuaternion(glm::vec2 start, glm::vec2 end) const;

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
	void beginRotation(float posX, float posY);

	/**
	 * Starts the quarternion rotation by setting the starting position
	 * @param pos - A two-dimensional cursor position
	 */
	void beginRotation(glm::vec2 pos);

	/**
	 * Applies the current position to the ongoing quaternion rotation
	 * @param posX - The x-coordinate of the current cursor position
	 * @param posY - The y-coordinate of the current cursor rotation
	 */
	virtual void rotate(float posX, float posY);

	/**
	 * Applies the current position to the ongoing quaternion rotation
	 * @param pos - The current two-dimensional cursor position
	 */
	virtual void rotate(glm::vec2 pos);

	/**
	 * Completes the quaternion arcball rotation
	 */
	void endRotation();

	/**
	 * Returns the quaternion rotation as a four-by-four matrix
	 */
	glm::mat4 getRotationMatrix();

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
	 * @param posX - The x-coordinate of a cursor position
	 * @param posY - The y-coordinate of a cursor position
	 * @param width - The width of the screen
	 * @param height - The height of the screen
	 * @return The cursor position in normaized device coordinates
	 */
	static glm::vec2 screenToNDC(double posX, double posY, int width, int height);

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