#ifndef CUBE_BUILDER_H
#define CUBE_BUILDER_H

#include<glm/glm.hpp>

#include<vector>
#include<array>
#include<cstdint>

#include"Mesh.h"
#include"TextureAtlas.h"
#include"Cube.h"

#define LEFT_BOTTOM_BACK	glm::vec3(0.0f, 0.0f, 0.0f)
#define RIGHT_TOP_BACK		glm::vec3(1.0f, 1.0f, 0.0f)
#define RIGHT_BOTTOM_BACK	glm::vec3(1.0f, 0.0f, 0.0f)
#define LEFT_TOP_BACK		glm::vec3(0.0f, 1.0f, 0.0f)

#define LEFT_BOTTOM_FRONT	glm::vec3(0.0f, 0.0f, 1.0f)
#define RIGHT_TOP_FRONT		glm::vec3(1.0f, 1.0f, 1.0f)
#define RIGHT_BOTTOM_FRONT	glm::vec3(1.0f, 0.0f, 1.0f)
#define LEFT_TOP_FRONT		glm::vec3(0.0f, 1.0f, 1.0f)

#define TEX_BOTTOM_LEFT(v)	glm::vec2(v.location.x, v.location.y)
#define TEX_BOTTOM_RIGHT(v)	glm::vec2(v.location.x + v.size.x, v.location.y)
#define TEX_TOP_LEFT(v)		glm::vec2(v.location.x, v.location.y + v.size.y)
#define TEX_TOP_RIGHT(v)	glm::vec2(v.location.x + v.size.x, v.location.y + v.size.y)

class Mesh;
class Cube;
struct Vertex;
struct Quad;

class CubeBuilder
{
public:
	enum class Face
	{
		BACK = 1 << 0, //	(-z)
		FRONT = 1 << 1, //	(+z)
		LEFT = 1 << 2, //	(-x)
		RIGHT = 1 << 3, //	(+x)
		TOP = 1 << 4, //	(+y)
		BOTTOM = 1 << 5 //	(-y)
	};

	/**
	 * @brief Loads the vertices based on given data
	 * 
	 * @param flags The binary value representing which faces will be shown (ie 00101011)
	 * @param quads The list of quads that the vertices will be added to
	 * @param tex The texture data for the vertices
	 */
	static void loadVertices(std::uint8_t flags, std::vector<Quad>& quads, const 
		tex::AtlasCollection_t& tex);

	/**
	 * @brief Loads the vertices based on given data
	 * 
	 * @param flags The binary value representing which faces will be shown (ie 00101011)
	 * @param tex The texture data for the vertices
	 * 
	 * @returns The cubes will all the vertice data
	 */
	static Cube loadVertices(std::uint8_t flags, const tex::AtlasCollection_t& tex);

	/**
	 * @brief Loads a specific face of the cube
	 * 
	 * @param tex The texture data for that specific face
	 * @param face The face that will be shown
	 * @return Quad 
	 */
	static Quad loadFace(const tex::AtlasCollection_t& tex, CubeBuilder::Face face);

	static Quad getBack(const Atlas& atlas);
	static Quad getFront(const Atlas& atlas);
	static Quad getLeft(const Atlas& atlas);
	static Quad getRight(const Atlas& atlas);
	static Quad getTop(const Atlas& atlas);
	static Quad getBottom(const Atlas& atlas);

	static Cube loadGrassVertices(const Atlas& atlas);

};


#endif // !CUBE_BUILDER_H
