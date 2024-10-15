#ifndef CAMERA_H
#define CAMERA_H

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "render/Renderer.h"
#include "render/Frustum.h"
#include "Settings.h"

class Camera
{
public:
	enum class Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};
		
private:
	// Canera Position
	glm::vec3 position;

	// Camera Statistics
	float yaw;
	float pitch;
	float sensitivity;

	// Movement Vectors
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	// Mouse
	bool firstTime;
	float lastX;
	float lastY;
	float lastFrame;

public:
	/**
	 * Intializes the camera to its default values
	 * 
	 * @param position The original x, y, z location of the camera
	 * 
	 */
	Camera(const glm::vec3& position);

	/**
	 * Gets the Cameras view matrix to be multiplied alongisde
	 * the projection matrix and model matrix 
	 * 
	 */
	glm::mat4 getViewMatrix();

	/**
	 * Changes where the player is looking based on the cursor position
	 *  
	 * @param distanceMoved Distance moved by the cursor relative 
	 * to the last frame
	 */
	void lookAround(game::CursorLocation_t distanceMoved);

	/**
	 * @returns The new x, y, z position of the camera
	 */
	glm::vec3 getPosition();
	/**
	 * Moves the camera to a new location
	 * 
	 * @param distanceToMove The amount of distance the player
	 * has moved denoted by an 3D vector
	 */
	void move(glm::vec3 distanceToMove);

	/**
	 * @returns The horizontal viewing angle of the player
	 */
	float getYaw() const;
	/**
	 * @returns The vertical viewing angle of the player
	 */
	float getPitch() const;

	/**
	 * @returns The 3D vector of where the player is facing
	 */
	glm::vec3 getDirectionVector();

	/**
	 * @returns The 3D vector parallel to where the player is facing
	 */
	glm::vec3 getFrontVector();
	/**
	 * @returns The 3D vector perpendicular to where the player is facing
	 */
	glm::vec3 getRightVector();

	/**
	 * Gets the viewing frustum of the player 
	 * (In order to clip objects outside view)
	 */
	Frustum generateFrustum();

private:
	/**
	 * Updates where the camera is facing based on its new yaw and pitch
	 */
	void updateMovementVectors();

};

#endif // !CAMERA_H

