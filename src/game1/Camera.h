#ifndef CAMERA_H
#define CAMERA_H

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include "render/Renderer.h"

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
	// Camera Default Values
	const float DEFAULT_YAW				= -90.0f;
	const float DEFAULT_PITCH			= 0.0f;
	const float DEFAULT_SENSITIVITY		= 0.1f;

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
	Camera(const glm::vec3& position);

	glm::mat4 getViewMatrix();

	void moveAround(Camera::Movement direction, float distance);
	void lookAround(float xMoveDifference, float yMoveDistance);

	glm::vec3 getPosition();
	void setPosition(float x, float y, float z);

	float getYaw() const;
	float getPitch() const;
	glm::vec3 getDirectionVector();

private:
	void updateMovementVectors();

};

#endif // !CAMERA_H

