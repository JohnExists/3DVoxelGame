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
	/**
	 * @brief Constructs and initializes a new Renderer object
	 * 
	 */
	Renderer();
	/**
	 * @brief Draws a mesh based on the shader
	 * 
	 * @param shader The shader reference that will determine the look of the mesh
	 * @param mesh The mesh pointer that will determine the vertices and shape
	 */
	void draw(Shader& shader, Mesh* mesh);

	/**
	 * @brief Renders the entire application
	 * 
	 * @param world Pointer to the world that the player is in
	 * @param camera Pointer to the players camera
	 * @param ui The players user interface
	 */
	void render(World* world, Camera* camera, GameInterface* ui);
	/**
	 * @brief Get the Shader At A String
	 * 
	 * @param shader The name of the shader
	 * @return Reference to shader with that name
	 */
	Shader& getShaderAt(std::string shader);
	/**
	 * @brief Binds the currently active shader
	 * 
	 * @param shaderType The new shader that will be binded
	 */
	void setShader(ShaderType shaderType);

};

#endif // !RENDERER_H