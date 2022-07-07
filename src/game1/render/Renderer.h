#ifndef RENDERER_H
#define RENDERER_H

#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<queue>
#include<stack>
#include<map>
#include<cstdint>

#include"Mesh.h"
#include"GLSL_Shaders.h"
#include"../world/World.h"
#include"../Camera.h"

class World;
class Mesh;
class Camera;

enum class ShaderType
{
	DEFAULT_SHADER,
	TEMPORARY_SHADER,
	MAX_SHADERS_COUNT
};

class Renderer
{
public:
	static constexpr float FOV = 80.0f;
	static constexpr float NEAR = 0.1f;
	static constexpr float FAR = 3000.0f;
	static constexpr float ASPECT_RATIO = static_cast<float>(960 / 540);

private:
	std::array<Shader, static_cast<int>(ShaderType::MAX_SHADERS_COUNT)> shaders{};
	ShaderType currentShader;
	World* world;
	Camera* camera;
public:
	Renderer(World* world, Camera* camera);
	void draw(Mesh* mesh);
	void render();
	Shader& getShader(ShaderType type);
	void setShader(ShaderType shaderType);
	void drawTempGUI();

};

#endif // !RENDERER_H

