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
	Camera(const glm::vec3& position);

	glm::mat4 getViewMatrix();

	void lookAround(game::CursorLocation_t distanceMoved);

	glm::vec3 getPosition();
	void move(glm::vec3 distanceToMove);

	float getYaw() const;
	float getPitch() const;

	glm::vec3 getDirectionVector();

	glm::vec3 getFrontVector();
	glm::vec3 getRightVector();

	Frustum generateFrustum();

private:
	void updateMovementVectors();

};

#endif // !CAMERA_H

