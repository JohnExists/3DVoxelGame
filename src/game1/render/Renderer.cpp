#include "Renderer.h"

Renderer::Renderer(World* world)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	currentShader = ShaderType::DEFAULT_SHADER;
	shaders[0] = Shader(DEF_VERTEX_SHADER, DEF_FRAG_SHADER);
	shaders[1] = Shader(TEMP_VERTEX_SHADER, TEMP_FRAG_SHADER);
	this->world = world;
}
void Renderer::draw(Mesh* mesh)
{
	Shader& current = getShader(currentShader);
	
	current.use();
	current.setMatrix("model", mesh->getModelMatrix());
	mesh->draw();
}

void Renderer::render()
{
	setShader(ShaderType::DEFAULT_SHADER);
	world->draw(*this);

	setShader(ShaderType::TEMPORARY_SHADER);
	drawTempGUI();
}

Shader& Renderer::getShader(ShaderType type)
{
	return shaders.at(static_cast<int>(type));
}

void Renderer::setShader(ShaderType shaderType)
{
	this->currentShader = shaderType;
}

void Renderer::drawTempGUI()
{
	static Texture texture("../res/crosshair.png");
	texture.setScalingFilter(GL_NEAREST, GL_NEAREST);
	texture.setWrapAround(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

	static std::vector<Quad> blockVertices{
		Quad{			// positions	// norms    // texture coords
			Vertex{ { -0.5f, -0.5f, 0.0f },		-1,	{ 0.0f, 0.0f} }, // bottom left
			Vertex{ {  0.5f, -0.5f, 0.0f },		-1,	{ 1.0f, 0.0f} }, // bottom right
			Vertex{ {  0.5f,  0.5f, 0.0f },		-1,	{ 1.0f, 1.0f} }, // top right
			Vertex{ {  0.5f,  0.5f, 0.0f },		-1,	{ 1.0f, 1.0f} }, // top right
			Vertex{ { -0.5f,  0.5f, 0.0f },		-1,	{ 0.0f, 1.0f} }, // top left 
			Vertex{ { -0.5f, -0.5f, 0.0f },		-1,	{ 0.0f, 0.0f} }, // bottom left
		}
	};

	static Mesh mesh(blockVertices, &texture);
	mesh.loadVertexObject();

	static bool scaled = false;
	if (!scaled) 
	{
		mesh.scale(0.025, 0.0425, 0);
		scaled = true;
	}

	texture.useSlot(getShader(ShaderType::TEMPORARY_SHADER), "texture1", 0);
	draw(&mesh);

}