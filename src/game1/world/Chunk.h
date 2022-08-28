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

	std::unique_ptr<Mesh> primary;
	std::unique_ptr<Mesh> secondary;
	std::unique_ptr<Mesh> tertiary;
	glm::vec2 position;
	
	std::unique_ptr<Block[]> blocks;
	std::unique_ptr<BiomeType[]> biomes;

	// std::array<Block, MAX_BLOCKS> blocks {};
	// std::array<BiomeType, MAX_XZ * MAX_XZ> biomes {};

	World* world = nullptr;
	BiomeLoader* loader = nullptr;

	Chunk* cacheFront;
	Chunk* cacheBehind;
	Chunk* cacheLeft;
	Chunk* cacheRight;

	AABB aabb;

public:
	Chunk(const glm::vec2& position, World* world, BiomeLoader& loader);

	void draw(Renderer& renderer, int meshToDraw);
	void buildBlocks();

	Block& getBlockAt(Location_t& positon);
	void setBlockAt(Location_t& position, BlockType newBlock, bool replaceAir);
	void ghostSetBlockAt(Location_t position, BlockType newBlock);

	BiomeType getBiomeAt(int x, int z);
	void setBiomeAt(int x, int z, BiomeType newBiome);

	const glm::vec2& getPosition() const;
	const AABB& getAABB() const;

	Chunk** getCachedChunk(char cachedDirection);
	void clearCache();
	void cacheChunks();

	Location_t toWorldCoordinatesAt(int x, int y, int z);
	LocalLocation_t toChunkCoordinatesAt(int x, int y, int z);

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