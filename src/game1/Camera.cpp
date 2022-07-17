#include "Camera.h"

//////////////////////////////////
/*		Public Functions		*/
//////////////////////////////////

Camera::Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f))
	: position{ position }
{
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

void Camera::moveAround(Movement direction, float distance)
{
	switch (direction)
	{
	case Movement::FORWARD:
		position.x += front.x * distance;
		position.z += front.z * distance;
		break;
	case Movement::BACKWARD:
		position.x -= front.x * distance;
		position.z -= front.z * distance;
		break;
	case Movement::LEFT:
		position -= right * distance;
		break;
	case Movement::RIGHT:
		position += right * distance;
		break;
	}
}

void Camera::lookAround(float xPos, float yPos)
{
	if (firstTime)
	{
		lastX = xPos;
		lastY = yPos;
		firstTime = false;
	}

	float xDifference = xPos - lastX;
	float yDifference = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	yaw += (xDifference * sensitivity);
	pitch += (yDifference * sensitivity);

	pitch = glm::clamp(pitch, -89.0f, 89.0f);
	updateMovementVectors();
}

glm::vec3 Camera::getPosition()
{
	return position;
}

void Camera::setPosition(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;

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

Frustum Camera::generateFrustum()
{
	const float HALF_V_SIDE = Renderer::FAR * tanf(Renderer::FOV * 0.8f);
	const float HALF_H_SIDE = HALF_V_SIDE * Renderer::ASPECT_RATIO;
	const glm::vec3 FRONT_MULT_FAR = Renderer::FAR * front;

	return Frustum(
		Plane( position, glm::cross(right, FRONT_MULT_FAR - up * HALF_V_SIDE) 	), // TOP
		Plane( position, glm::cross(FRONT_MULT_FAR + up * HALF_V_SIDE, right) 	), // BOTTOM

		Plane( position, glm::cross(up, FRONT_MULT_FAR + right * HALF_H_SIDE) 	), // RIGHT
		Plane( position, glm::cross(FRONT_MULT_FAR - right * HALF_H_SIDE, up) 	), // LEFT
		
		Plane( position + FRONT_MULT_FAR, -front 								), // FAR
		Plane( position + Renderer::NEAR * front, front 						)  // NEAR
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


