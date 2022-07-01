#ifndef MESH_H
#define MESH_H

#include<list>
#include<iostream>
#include <glad/glad.h>
#include<initializer_list>
#include<glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Shader.h"
#include "GLObject.h"

class VertexArrayObject;
struct Vertex;
struct Quad;

class Mesh
{
protected:
	std::vector<Quad>		quads;
	std::vector<char>		indices;
	const Texture*			texture;
	glm::mat4 				modelMatrix;
	VertexArrayObject* vao = nullptr;
public:
	Mesh(
		const std::vector<Quad>			quads,
		const Texture*					texture
	);

	Mesh(const Texture* texture);
	~Mesh();
	bool isLoaded();
	void clearVertexObject();

	void scale(float x, float y, float z);
	void translate(float x, float y, float z);
	//void rotate();

	void draw();

	void loadVertexObject();

	std::vector<Quad>& getQuads();

	void setModelMatrix(const glm::mat4 model);
	glm::mat4& getModelMatrix();


};

#endif