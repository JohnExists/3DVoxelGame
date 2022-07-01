#ifndef GL_OBJECT_H
#define GL_OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include<vector>
#include<array>

#include"../world/Block.h"

class Block;

struct Vertex
{
	glm::vec3 position;
	int blockFace;
	glm::vec2 texCoords;
};

struct Quad
{
	std::array<Vertex, 6> vertices;
};

class GLObject
{
protected:
	GLuint data = 0;

public:
	virtual void use() = 0;
	virtual void abandon() = 0;

};

class VertexObject : public GLObject
{
public:
	VertexObject(std::vector<Quad>* vertices);

	// Inherited via GLObject
	virtual void use() override;
	virtual void abandon() override;
};

class VertexArrayObject : public GLObject
{
private:
	VertexObject* vertexData;
public:
	VertexArrayObject(std::vector<Quad>* vertices);
	~VertexArrayObject();
	// Inherited via GLObject
	virtual void use() override;
	virtual void abandon() override;

private:
	void link();
	void linkData(int slot, int size, void* offset);

};

#endif // !GL_OBJECT_H