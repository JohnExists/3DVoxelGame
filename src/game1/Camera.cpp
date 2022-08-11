#include "Camera.h"

//////////////////////////////////
/*		Public Functions		*/
//////////////////////////////////

Camera::Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f))
	: position{ position }
{
	using namespace game;
	yaw				= DEFAULT_YAW;
	pitch			= DEFAULT_PITCH;
	sensitivity		= DEFAULT_SENSITIVITY;
	worldUp			= glm::vec3(0.0f, 1.0f, 0.0f);
	firstTime		= true;
	updateMovementVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::lookAround(game::CursorLocation_t distanceMoved)
{
	yaw += (distanceMoved.x * sensitivity);
	pitch += (distanceMoved.y * sensitivity);

	pitch = glm::clamp(pitch, -89.0f, 89.0f);
	updateMovementVectors();
}

glm::vec3 Camera::getPosition()
{
	return position;
}

void Camera::move(glm::vec3 distanceToMove)
{
	position += distanceToMove;
}

float Camera::getYaw() const
{
	return yaw;
}

float Camera::getPitch() const
{
	return pitch;
}

glm::vec3 Camera::getDirectionVector()
{
	return front;
}

glm::vec3 Camera::getFrontVector()
{
	return glm::normalize(
			glm::vec3(
				cos(glm::radians(yaw)),
				0,
				sin(glm::radians(yaw))
			)
		);
}

glm::vec3 Camera::getRightVector()
{
	return right;	
}

Frustum Camera::generateFrustum()
{
	using namespace game;
	const float HALF_V_SIDE = FAR * tanf(FOV * 0.8f);
	const float HALF_H_SIDE = HALF_V_SIDE * ASPECT_RATIO;
	const glm::vec3 FRONT_MULT_FAR = FAR * front;

	return Frustum(
		Plane( position, glm::cross(right, FRONT_MULT_FAR - up * HALF_V_SIDE) 	), // TOP
		Plane( position, glm::cross(FRONT_MULT_FAR + up * HALF_V_SIDE, right) 	), // BOTTOM

		Plane( position, glm::cross(up, FRONT_MULT_FAR + right * HALF_H_SIDE) 	), // RIGHT
		Plane( position, glm::cross(FRONT_MULT_FAR - right * HALF_H_SIDE, up) 	), // LEFT
		
		Plane( position + FRONT_MULT_FAR, -front 								), // FAR
		Plane( position + NEAR * front, front 									)  // NEAR
	);
}

//////////////////////////////////
/*		Private Functions		*/
//////////////////////////////////

void Camera::updateMovementVectors()
{
	front = glm::normalize(
		glm::vec3(
			cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
			sin(glm::radians(pitch)),
			sin(glm::radians(yaw)) * cos(glm::radians(pitch))
		)
	);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}


