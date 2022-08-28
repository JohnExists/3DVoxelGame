#ifndef MESH_H
#define MESH_H

#include<list>
#include<iostream>
#include<glad/glad.h>
#include<initializer_list>
#include<glm/gtc/matrix_transform.hpp>
#include<utility>

#include"Texture.h"
#include"Shader.h"
#include"GLObject.h"
#include"CubeBuilder.h"
#include"../Settings.h"

class VertexArrayObject;
class Cube;
struct Vertex;
struct Quad;

class Mesh
{
protected:
	std::vector<Quad>					quads;
	const Texture*						texture;
	glm::mat4 							modelMatrix;
	std::unique_ptr<VertexArrayObject> 	vao;
	Shader*								shader;
public:
	Mesh(const Texture* texture, Shader* shader);

	bool isLoaded();
	void clearVertexObject();

	void scale(float x, float y, float z);
	void translate(float x, float y, float z);
	void translate(glm::vec3 translation);
	void rotate(float x, float y, float z, float angle);
	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);

	void addCube(Cube cube);
	void addRect(glm::vec2 position, glm::vec2 size, Atlas atlas);

	void clear();
	void draw();

	void loadVertexObject();

	std::vector<Quad>& getQuads();
	void addQuad(Quad quad);

	void setModelMatrix(const glm::mat4 model);
	glm::mat4& getModelMatrix();

	Shader& getShader();

};

#endif