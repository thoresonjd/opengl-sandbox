/**
 * @file camera.h
 * @brief Flying free look camera
 * @date October 2023
 */

#ifndef CAMERA_H
#define CAMERA_H
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @class Camera - Camera that can look and move around freely
 */
class Camera {
private:
	static constexpr float MIN_PITCH = -89.99f;
	static constexpr float MAX_PITCH = 89.99f;
	static constexpr float MIN_FOV = 1.0f;
	static constexpr float MAX_FOV = 45.0f;
	static constexpr float DEFAULT_FOV = 45.0f;
	static constexpr float DEFAULT_PITCH = 0.0f;
	static constexpr float DEFAULT_YAW = -90.0f;
	static constexpr float MOVEMENT_SPEED = 5.0f;
	static constexpr float LOOK_SENSITIVITY = 0.1f;
	glm::vec3 initialPosition;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldUp;
	float pitch;
	float yaw;
	float fieldOfView;
	
	/**
	 * Recomputes camera vectors: front, right, and up
	 */
	void updateCameraVectors();
public:

	/**
	 * Represents directions that the camera may move
	 */
	enum class Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};
	
	/**
	 * Default constructor
	 * @param position - The camera's position
	 * @param worldUp - The upward direction relative to the world
	 * @param pitch - The angle around the horizontal axis (x-axis or z-axis)
	 * @param yaw - The angle around the vertical axis (y-axis)
	 */
	Camera(
		glm::vec3 position = glm::vec3(0.0f),
		glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
		float pitch = DEFAULT_PITCH,
		float yaw = DEFAULT_YAW
	);

	/**
	 * Constructor
	 * @param posX - The x-coordinate of the camera's position
	 * @param posY - The y-coordinate of the camera's position
	 * @param posZ - The z-coordinate of the camera's position
	 * @param worldUpX - The x-coordinate of the world's upward direction
	 * @param worldUpY - The y-coordinate of the world's upward direction
	 * @param worldUpZ - The z-coordinate of the world's upward direction
	 * @param pitch - The angle around the horizontal plane (xz-plane)
	 * @param yaw - The angle around the vertical axis (y-axis)
	 */
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
	 * Processes changes in the position the camera is located.
	 * Movement can be conducted via keyboard input.
	 * @param direction - The direction in which to move the camera
	 * @param deltaTime - The change in time to apply to movement velocity
	 */
	void move(Movement direction, float deltaTime);

	/**
	 * Processes changes in the direction in which the camera is looking.
	 * Looking can be adjusted via moving the mouse.
	 * @param offsetX - Amount in which the camera has looked along the x-axis
	 * @param offsetY - Amount in which the camera has looked along the y-axis
	 * @param constrainPitch - Determines if the pitch should be constrained within a safe range
	 */
	void look(float offsetX, float offsetY, bool constrainPitch = true);

	/**
	 * Processes changes in field of view.
	 * Field of view can be adjusted via mouse scroll.
	 * @param offset - Amount in which the field of view has changed
	 */
	void adjustFOV(float offset);
	
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
	 * Retrives the position of the camera
	 */
	glm::vec3 getPosition() const;
};

#endif