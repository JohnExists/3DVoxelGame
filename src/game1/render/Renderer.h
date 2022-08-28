#ifndef RENDERER_H
#define RENDERER_H

#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<queue>
#include<stack>
#include<map>
#include<cstdint>
#include<unordered_map>

#include"Mesh.h"
#include"../world/World.h"
#include"../Camera.h"
#include"Frustum.h"
#include"../Settings.h"
#include"../ui/GameInterface.h"

class World;
class Mesh;
class Camera;
class GameInterface;

enum class ShaderType
{
	DEFAULT_SHADER,
	TEMPORARY_SHADER,
	MAX_SHADERS_COUNT
};

class Renderer
{
private:
	std::unordered_map<std::string, Shader> shaders;
	ShaderType currentShader;
	Camera* camera;
public:
	Renderer();
	void draw(Shader& shader, Mesh* mesh);

	void render(World* world, Camera* camera, GameInterface* ui);
	Shader& getShaderAt(std::string shader);
	void setShader(ShaderType shaderType);
	void drawTempGUI();

};

#endif // !RENDERER_H