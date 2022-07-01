#ifndef RENDERER_H
#define RENDERER_H

#include<glm/glm.hpp>

#include<queue>
#include<stack>
#include<map>
#include<cstdint>

#include"Mesh.h"
#include"GLSL_Shaders.h"
#include"../world/World.h"

class World;
class Mesh;
enum class ShaderType
{
	DEFAULT_SHADER,
	TEMPORARY_SHADER,
	MAX_SHADERS_COUNT
};

class Renderer
{
private:
	std::array<Shader, static_cast<int>(ShaderType::MAX_SHADERS_COUNT)> shaders{};
	ShaderType currentShader;
	World* world;
public:
	Renderer(World* world);
	void draw(Mesh* mesh);
	void render();
	Shader& getShader(ShaderType type);
	void setShader(ShaderType shaderType);
	void drawTempGUI();

};

#endif // !RENDERER_H

