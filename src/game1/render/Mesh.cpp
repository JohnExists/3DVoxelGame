#include "Mesh.h"

//////////////////////////////////
/*		Public Functions		*/
//////////////////////////////////

Mesh::Mesh(const Texture* texture, Shader* shader) : texture(texture), shader(shader)
{
	this->quads.reserve(1000);
}

bool Mesh::isLoaded() 
{
	return vao.get() == nullptr;
}

void Mesh::clearVertexObject() 
{
	vao.reset();
}

void Mesh::scale(float x, float y, float z)
{
	modelMatrix = glm::scale(modelMatrix, glm::vec3(x, y, z));
}

void Mesh::translate(float x, float y, float z)
{
	modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));
}

void Mesh::translate(glm::vec3 translation)
{
	translate(translation.x, translation.y, translation.z);	
}

void Mesh::rotate(float x, float y, float z, float angle)
{
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(x, y, z));
}

void Mesh::rotateX(float angle)
{
	rotate(1.0f, 0.0f, 0.0f, angle);
}

void Mesh::rotateY(float angle)
{
	rotate(0.0f, 1.0f, 0.0f, angle);
}

void Mesh::rotateZ(float angle)
{
	rotate(0.0f, 0.0f, 1.0f, angle);
}

void Mesh::addCube(Cube cube)
{
	for (auto &&quad : cube.getQuads())
	{
		addQuad(quad);
	}

}

void Mesh::addRect(glm::vec2 position, glm::vec2 size, Atlas atlas)
{
	float x = position.x;
	float y = position.y;
	float halfX = size.x / 2;
	float halfY = size.y / 2;

	float texX = atlas.location.x;
	float texY = atlas.location.y;
	float texSizeX = atlas.size.x;
	float texSizeY = atlas.size.y;

	glm::vec3 bottomLeft    (x - halfX, y - halfY, -0.1f);
    glm::vec3 topLeft       (x - halfX, y + halfY, -0.1f);
    glm::vec3 bottomRight   (x + halfX, y - halfY, -0.1f);
    glm::vec3 topRight      (x + halfX, y + halfY, -0.1f);

	addQuad(
        Quad{   // positions        // texture coords
            Vertex{ bottomLeft,  -1, { texX, texY }   					  },
            Vertex{ bottomRight, -1, { texX + texSizeX, texY }  		  },
            Vertex{ topRight,    -1, { texX + texSizeX, texY + texSizeY } },
            Vertex{ topRight,    -1, { texX + texSizeX, texY + texSizeY } },
            Vertex{ topLeft,     -1, { texX, texY + texSizeY }   		  },
            Vertex{ bottomLeft,  -1, { texX, texY }   					  },
        }
    );

}


void Mesh::clear()
{
	quads.clear();
}

void Mesh::draw()
{
	if(quads.empty()) return;
	if(!vao) loadVertexObject();

	shader->use();
	(*shader)["model"] = modelMatrix;

	texture->useSlot(*shader, "texture1", 0);

	if(vao) vao->draw();
}

void Mesh::addQuad(Quad quad)
{
	quads.push_back(quad);	
}

void Mesh::loadVertexObject()
{
	if (quads.empty()) return;

	clearVertexObject();
	vao = std::make_unique<VertexArrayObject>(&this->quads);
}

std::vector<Quad>& Mesh::getQuads()
{
	return quads;
}

void Mesh::setModelMatrix(const glm::mat4 model)
{
	this->modelMatrix = model;
}

glm::mat4& Mesh::getModelMatrix()
{
	return modelMatrix;
}

Shader& Mesh::getShader()
{
	return *shader;	
}

