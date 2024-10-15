#ifndef CHUNK_H
#define CHUNK_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <chrono>
#include <optional>
#include <utility>

#include "../Random.h"
#include "../render/Texture.h"
#include "../render/Renderer.h"
#include "../render/Mesh.h"
#include "../render/CubeBuilder.h"
#include "../render/Frustum.h"
#include "../AABB.h"

#include "biome/Biome.h"
#include "biome/BiomeLoader.h"
#include "FastNoise.h"
#include "Block.h"
#include "World.h"
#include "Decoration.h"

class BiomeLoader;
class Biome;
class BlockBuilder;
class Renderer;
class World;
class Block;
class Mesh;

class Chunk
{
public:
	static const int MAX_XZ = 16;
	static const int MAX_Y = 256;
	static const int MAX_BLOCKS = MAX_XZ * MAX_Y * MAX_XZ;

	static const int PRIMARY_MESH = 0;
	static const int SECONDARY_MESH = 1;
	static const int TERTIARY_MESH = 2;

private:
	using Location_t = glm::vec3;
	using LocalLocation_t = glm::vec3;

	// Storages of all the meshes for 3D rendering
	std::unique_ptr<Mesh> primary;
	std::unique_ptr<Mesh> secondary;
	std::unique_ptr<Mesh> tertiary;

	// location of the chunk relative to the world
	glm::vec2 position;
	
	// The list of all the blocks in the chunk
	std::unique_ptr<Block[]> blocks;
	// The list of all biomes in the chunk
	std::unique_ptr<BiomeType[]> biomes;

	// World that the chunk is in
	World* world = nullptr;
	// The class that handles biome and natural world generation
	BiomeLoader* loader = nullptr;

	// Pointer to all surrounding chunks
	Chunk* cacheFront;
	Chunk* cacheBehind;
	Chunk* cacheLeft;
	Chunk* cacheRight;

	// The chunks colission
	AABB aabb;
public:
	/**
	 * @brief Initializes the chunk to its default properties
	 * 
	 * @param position 2D position relative to the world
	 * @param world The world that the chunk is in
	 * @param loader BiomeLoader reference used for natural world generation
	 */
	Chunk(const glm::vec2& position, World* world, BiomeLoader& loader);

	/**
	 * @brief Renders the specific chunk
	 * 
	 * @param renderer The renderer object that will be used to draw the chunk
	 * @param meshToDraw Which mesh this function show draw (PRIMARY_MESH or SECONDARY MESH)
	 */
	void draw(Renderer& renderer, int meshToDraw);
	/**
	 * @brief Generates all the blocks for this speicfic chunk
	 */
	void buildBlocks();

	/**
	 * @param position The x, y, z location of the block that will be returned
	 * @returns The block at that specific location
	 */
	Block& getBlockAt(Location_t& positon);
	/**
	 * @brief Places a block at a certain location
	 * 
	 * @param position The x, y, z coordinates of the block that will be set
	 * @param newBlock The new block that will be placed at that position
	 * @param replaceAir If true, replaces the block even if its air, doesn't remove if false
	 */
	void setBlockAt(Location_t& position, BlockType newBlock, bool replaceAir);

	/**
	 * @brief Places a block at a certain location without updating the mesh
	 * 
	 * @param position The position of that new block
	 * @param newBlock The block that will be placed
	 */
	void ghostSetBlockAt(Location_t position, BlockType newBlock);

	/**
	 * @param x The x coordinates relative to the world
	 * @param z The z coordinates relative to the world
	 * @returns the biome at those specific coordinates
	 */
	BiomeType getBiomeAt(int x, int z);

	/**
	 * @param x The x coordinates relative to the world
	 * @param z The z coordinates relative to the world
	 * @param newBiome The new biome that will be set
	 */
	void setBiomeAt(int x, int z, BiomeType newBiome);

	/**
	 * @returns The position of the chunk relative to the world
	 */
	const glm::vec2& getPosition() const;
	/**
	 * @returns The colission box of the chunk
	 */
	const AABB& getAABB() const;

	/**
	 * @brief Obtains pointer of a nearby chunk
	 * 
	 * @param cachedDirection The direction of the nearby chunk ('F' for front, etc)
	 * @returns The pointer to a nearby chunk
	 */
	std::optional<Chunk*> getCachedChunk(char cachedDirection);
	/**
	 * @brief Deletes pointer to nearby chunks
	 */
	void clearCache();
	/**
	 * @brief Regenerates pointers to nearby chunks
	 */
	void cacheChunks();

	/**
	 * @brief Converts world coordinates to chunk coordinates
	 * @return The world coordinates based on chunk (x, y, z)
	 */
	Location_t toWorldCoordinatesAt(int x, int y, int z);

	/**
	 * @brief Converts chunk coordinates to world coordinates
	 * @return The chunk coordinates based on world (x, y, z)
	 */	
	LocalLocation_t toChunkCoordinatesAt(int x, int y, int z);

	/**
	 * @brief Places a piece of decoration in the chunk (TREES, GRASS_BLADES, etc)
	 * 
	 * @param decoration The decoration item that will be placed in the chunk
	 * @param xyz The location of that decoration
	 */
	void loadDecoration(Decoration decoration, int x, int y, int z);

private:

	void loadMatrix(Renderer& renderer);
	void loadBlocks();

	BlockType getBlockTypeAt(int x, int y, int z);
	BlockType getBlockTypeAt(LocalLocation_t position);

	bool shouldRender(const LocalLocation_t& position, bool isBlockWater);

	std::uint8_t getFlags(int x, int y, int z);
	void buildBlockAt(int x, int y, int z);

	Block& getLocalBlockAt(int x, int y, int z) const;
	Block& getLocalBlockAt(LocalLocation_t position) const;


	bool isBlockWithinBounds(int x, int z);


};


#endif // !CHUNK_H