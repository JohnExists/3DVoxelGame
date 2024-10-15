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
	// All values for the mesh
	std::vector<Quad>					quads;
	const Texture*						texture;
	glm::mat4 							modelMatrix;
	std::unique_ptr<VertexArrayObject> 	vao;
	Shader*								shader;
public:
	Mesh(const Texture* texture, Shader* shader);

	/**
	 * @returns True if all the values of the mesh are not null
	 */
	bool isLoaded();
	/**
	 * @brief Clears all vertices from this mesh
	 */
	void clearVertexObject();

	/**
	 * @brief Changes the size of the mesh
	 * 
	 * @param xyz The amount to scale in that specific direction
	 */
	void scale(float x, float y, float z);
	/**
	 * @brief Moves the mesh by a specific distance
	 * 
	 * @param xyz The values to be moved in each direction
	 */
	void translate(float x, float y, float z);
	/**
	 * @brief Moves the mesh by a specific distance
	 * 
	 * @param xyz The values to be moved in each direction
	 */
	void translate(glm::vec3 translation);
	/**
	 * @brief Rotates the mesh around itslef
	 * 
	 * @param xyz Scale on how to rotated along each axis
	 * @param angle The angle that it will be rotated (in degrees)
	 */
	void rotate(float x, float y, float z, float angle);
	
	/**
	 * @brief Rotates along the x y and z-axis
	 * 
	 * @param angle The angle it will be rotated by
	 */
	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);

	/**
	 * @brief Adds a new cube object to the mesh
	 * 
	 * @param cube The cube object that will be added
	 */
	void addCube(Cube cube);
	/**
	 * @brief Adds a new rect object to the mesh
	 * 
	 * @param position The 2 position of the rectangle
	 * @param size The size of the rectangle
	 * @param atlas The texture atlas for this rectangle
	 */
	void addRect(glm::vec2 position, glm::vec2 size, Atlas atlas);

	/**
	 * @brief Clears all objects from this mesh
	 */
	void clear();
	/**
	 * @brief Renders all objects on this mesh to the screen
	 */
	void draw();

	/**
	 * @brief Binds the current vertex object to be drawn
	 */
	void loadVertexObject();

	/**
	 * @return Returns all the quads in this mesh
	 */
	std::vector<Quad>& getQuads();
	/**
	 * @brief Adds a new quad object to the mesh
	 * 
	 * @param quad The quad that will be added
	 */
	void addQuad(Quad quad);

	/**
	 * @brief Set the Model Matrix object
	 * 
	 * @param model The new model matrix that will be set to this mesh
	 */
	void setModelMatrix(const glm::mat4 model);
	/**
	 * @return The reference to Model Matrix object
	 */
	glm::mat4& getModelMatrix();

	/**
	 * @return Returns a reference to the shader object
	 */
	Shader& getShader();

};

#endif