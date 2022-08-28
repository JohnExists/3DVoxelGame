#include "Renderer.h"

Renderer::Renderer()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	currentShader = ShaderType::DEFAULT_SHADER;
	
	for (auto &&[shaderName, shaderLayout] : game::gameShaders)
	{
		shaders[shaderName] = Shader(shaderName);
	}
}

void Renderer::draw(Shader& shader, Mesh* mesh)
{
	shader.use();
	shader["model"] = mesh->getModelMatrix();

	mesh->draw();
}

void Renderer::render(World* world, Camera* camera, GameInterface* ui)
{
	using namespace game;
	glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(FOV), ASPECT_RATIO, NEAR, FAR);

	setShader(ShaderType::DEFAULT_SHADER);
	Shader& shader = getShaderAt("default_world");
	shader.use();
	shader["minFog"] 		= World::RENDER_DISTANCE * 8.5f;
	shader["projection"] 	= projectionMatrix;
	shader["view"] 			= camera->getViewMatrix();
	shader["cameraPos"] 	= camera->getPosition();

	Shader& shader2 = getShaderAt("block_hotbar_select");
	shader2.use();
	shader2["projection"] 	= projectionMatrix;
	shader2["view"] 		= ui->getCamera().getViewMatrix();

	Shader& shader3 = getShaderAt("user_interface");
	shader3.use();
	shader3["projection"] 	= projectionMatrix;
	shader3["view"] 		= ui->getCamera().getViewMatrix();

	
	Frustum frustum = camera->generateFrustum();
	world->draw(*this, frustum);
	ui->draw(*this);	
}

Shader& Renderer::getShaderAt(std::string shader)
{
	auto search = shaders.find(shader);

	if(search != shaders.end()) return search->second;
	throw("Failed to find shader: " + shader);
}

void Renderer::setShader(ShaderType shaderType)
{
	this->currentShader = shaderType;
}