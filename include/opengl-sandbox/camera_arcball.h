/**
 * @file camera_arcball.h
 * @brief Orbital camera derived from an arcball
 * @date October 2023
 */

#ifndef CAMERA_ARCBALL_H
#define CAMERA_ARCBALL_H
#pragma once

#include <opengl-sandbox/arcball.h>

/**
 * @class CameraArcball - Orbital camera derived from an arcball
 */
class CameraArcball : public Arcball {
private:

	static constexpr float MIN_FOV = 1.0f;
	static constexpr float MAX_FOV = 45.0f;
	static constexpr float TRANSLATION_SPEED = 7.0f;
	static constexpr glm::vec3 ORIGIN = glm::vec3(0.0f);
	static constexpr glm::vec3 Y_POSITIVE_AXIS = glm::vec3(0.0f, 1.0f, 0.0f);
	static constexpr glm::vec3 Z_POSITIVE_AXIS = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 target;
	glm::vec3 worldUp;
	float fieldOfView;
	bool translating = false;

	/**
	 * Recomputes camera vectors: front, right, and up
	 */
	void updateCameraVectors();

public:

	/**
	 * Default constructor
	 * @param position - The camera's position
	 * @param target - The target's position
	 * @param worldUp - The upward direction relative to the world
	 */
	CameraArcball(
		glm::vec3 position = Z_POSITIVE_AXIS,
		glm::vec3 target = ORIGIN,
		glm::vec3 worldUp = Y_POSITIVE_AXIS
	);

	/**
	 * Constructor
	 * @param posX - The x-coordinate of the camera's position
	 * @param posY - The y-coordinate of the camera's position
	 * @param posZ - The z-coordinate of the camera's position
	 * @param targetX - The x-coordinate of the target's position
	 * @param targetY - The y-coordinate of the target's position
	 * @param targetZ - The z-coordinate of the target's position
	 * @param worldUpX - The x-coordinate of the world's upward direction
	 * @param worldUpY - The y-coordinate of the world's upward direction
	 * @param worldUpZ - The z-coordinate of the world's upward direction
	 */
	CameraArcball(
		float posX, float posY, float posZ,
		float targetX, float targetY, float targetZ,
		float worldUpX, float worldUpY, float worldUpZ
	);

	CameraArcball(const CameraArcball& other) = delete;
	CameraArcball(CameraArcball&& other) = delete;
	CameraArcball& operator=(const CameraArcball& other) = delete;
	CameraArcball& operator=(CameraArcball&& other) = delete;
	~CameraArcball() = default;

	/**
	 * Starts the camera translation
	 */
	void beginTranslation();

	/**
	 * Translates the camera forward or backward
	 * @param offset - Amount in which to translate
	 */
	void translate(float offset);

	/**
	 * Completes the camera translation
	 */
	void endTranslation();

	/**
	 * Determines if the arcball is actively undergoing a rotation
	 */
	bool isTranslating() const;

	/**
	 * Processes changes in field of view.
	 * Field of view can be adjusted via mouse scroll.
	 * @param offset - Amount in which the field of view has changed
	 */
	void adjustFOV(float offset);

	/**
	 * Computes the view matrix for the camera given it's position, front, and up vectors
	 */
	glm::mat4 getViewMatrix();

	/**
	 * Retrieves the camera's field of view (zoom)
	 */
	float getFOV() const;

	/**
	 * Retrives the position of the camera
	 */
	glm::vec3 getPosition();
};

#endif