#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <thread>

#include "../game1/world/World.h"
#include "GameState.h"
#include "InputHandler.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const int SCR_WIDTH = 960;
const int SCR_HEIGHT = 540;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera* cameraPtr;


int main(int argc, char const** argv)
{
	
	std::cout << "Starting Thread... " << std::this_thread::get_id() << '\n';

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learning OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << '\n';
		glfwTerminate(); 
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << '\n';
		return EXIT_FAILURE;
	}

	GameState gameState;
	InputHandler::launchInputHandler(window);

	cameraPtr = &gameState.getPlayerCamera();

	double previousTime = glfwGetTime();

	int frameCount = 0;

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop\watch
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		InputHandler::update();
		processInput(window);

		gameState.update(deltaTime);

		InputHandler::reset();

		glfwSwapBuffers(window);
		glfwPollEvents();

		double currentTime = glfwGetTime();
		frameCount++;
		// If a second has passed.
		if (currentTime - previousTime >= 1.0)
		{
			// Display the frame count here any way you want.
			glm::vec3 xx = cameraPtr->getPosition();
			glm::vec3 xz = cameraPtr->getDirectionVector();


			std::string str = "Learning OpenGL ";
			std::ostringstream stream;
			stream << "LearningOpenGL "
					"X: " << xx.x << ", Y: " << xx.y << ", Z: " << xx.z <<
				" :: DirX: " << xz.x << ", DirY: " << xz.y << ", DirZ: " << xz.z
				<< " :: FPS: " << frameCount;

			glfwSetWindowTitle(window, stream.str().c_str());
			frameCount = 0;
			previousTime = currentTime;
		}
	}
	gameState.disable();
	glfwTerminate();
	return EXIT_SUCCESS;
}

void processInput(GLFWwindow *window)
{
	if(InputHandler::keyIsPressed(GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}