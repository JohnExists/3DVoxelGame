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
	//std::cout << y << '\n';
	position.x += x;
	position.y += y;
	position.z += z;
	//std::cout << y << '\n';

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


