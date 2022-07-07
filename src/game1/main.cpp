#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include "../game1/world/World.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

// settings
const int SCR_WIDTH = 960;
const int SCR_HEIGHT = 540;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(0.0f, 75.0f, 3.0f));
World *worldPtr;

int main(int argc, char const **argv)
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

	double startTime = glfwGetTime();

	World world(399393994);
	Renderer renderer(&world, &camera);

	worldPtr = &world;

	double previousTime = glfwGetTime();
	std::cout << previousTime - startTime << '\n';

	int frameCount = 0;

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	std::thread chunkBuildThread([&]() {
		std::cout << "Starting Thread... " << std::this_thread::get_id() << '\n';
		while (!glfwWindowShouldClose(window))
		{
			world.updateChunksBuilds(&camera, 0);
		}
	});

	std::thread chunkDeletionThread([&]() {
		std::cout << "Starting Thread... " << std::this_thread::get_id() << '\n';
		while (!glfwWindowShouldClose(window))
		{
			world.updateChunksBuilds(&camera, 1);
		}
	});

	std::thread chunkMeshingThread([&]() {
		std::cout << "Starting Thread... " << std::this_thread::get_id() << '\n';
		while (!glfwWindowShouldClose(window))
		{
			world.updateChunksMeshing();
		}
	});

	chunkBuildThread.detach();
	chunkDeletionThread.detach();
	chunkMeshingThread.detach();


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);

		// render
		// ------
		glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Shader &shader = renderer.getShader(ShaderType::DEFAULT_SHADER);

		shader.use();
		shader.setFloat("minFog", 10 * 8.5f);

		renderer.render();

		glfwSwapBuffers(window);
		glfwPollEvents();

		double currentTime = glfwGetTime();
		frameCount++;
		// If a second has passed.
		if (currentTime - previousTime >= 1.0)
		{
			// Display the frame count here any way you want.
			std::string str = "Learning OpenGL ";
			str.append(std::to_string(frameCount))
				.append(" X: " + std::to_string((camera.getPosition().x)) + " ")
				.append("Y: " + std::to_string((camera.getPosition()).y) + " ")
				.append("Z: " + std::to_string((camera.getPosition()).z) + " ")
				.append(" Xdir: " + std::to_string(camera.getDirectionVector().x) + " ")
				.append("Ydir: " + std::to_string(camera.getDirectionVector().y) + " ")
				.append("Zdir: " + std::to_string(camera.getDirectionVector().z));
			glfwSetWindowTitle(window, str.c_str());
			frameCount = 0;
			previousTime = currentTime;
		}
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float movementSpeed = 25;

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		movementSpeed = 45;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.moveAround(Camera::Movement::FORWARD, movementSpeed * deltaTime);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.moveAround(Camera::Movement::BACKWARD, movementSpeed * deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.moveAround(Camera::Movement::LEFT, movementSpeed * deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.moveAround(Camera::Movement::RIGHT, movementSpeed * deltaTime);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.setPosition(0, movementSpeed * deltaTime, 0);

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.setPosition(0, -movementSpeed * deltaTime, 0);
}

void mouse_callback(GLFWwindow *window, double x, double y)
{
	camera.lookAround(static_cast<float>(x), static_cast<float>(y));
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		worldPtr->castRay(camera, true);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		worldPtr->castRay(camera, false);
	}
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}