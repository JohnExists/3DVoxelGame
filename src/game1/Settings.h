#ifndef SETTINGS_H
#define SETTINGS_H

#include<GLFW/glfw3.h>
#include<tuple>
#include<string>

namespace game
{
    using MouseStatus_t = std::pair<int, int>;
	using Location_t = glm::vec3;
	using ChunkLocation_t = glm::vec2;
	using CursorLocation_t = glm::vec2;

	// Window Settings
	constexpr float WINDOW_WIDTH		= 960.0f;
	constexpr float WINDOW_HEIGHT		= 540.0f;

	// Camera Default Settings
    constexpr float DEFAULT_YAW			= -90.0f;
	constexpr float DEFAULT_PITCH		= 0.0f;
	constexpr float DEFAULT_SENSITIVITY	= 0.1f;

	// Frustum Default Settings
	constexpr float FOV 				= 80.0f;
	constexpr float NEAR 				= 0.1f;
	constexpr float FAR 				= 500.0f;
	constexpr float ASPECT_RATIO 		= WINDOW_WIDTH / WINDOW_HEIGHT;

	// Controls Settings
	constexpr int MOVE_FORWARD			= GLFW_KEY_W;
	constexpr int MOVE_BACKWARD			= GLFW_KEY_S;
	constexpr int STRAFE_LEFT			= GLFW_KEY_A;
	constexpr int STRAFE_RIGHT			= GLFW_KEY_D;
	constexpr int SPRINT				= GLFW_KEY_LEFT_CONTROL;
	constexpr int JUMP_OR_FLY_UP		= GLFW_KEY_SPACE;
	constexpr int CROUCH_OR_FLY_DOWN	= GLFW_KEY_LEFT_SHIFT;
	constexpr int CLOSE_WINDOW 			= GLFW_KEY_ESCAPE;

	constexpr MouseStatus_t PUNCH		= MouseStatus_t(GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE);
	constexpr MouseStatus_t PLACE		= MouseStatus_t(GLFW_MOUSE_BUTTON_RIGHT, GLFW_RELEASE);

	std::string availableShaders = {
		"default_world",
		"user_interface"
	};
}

#endif // SETTINGS_H
