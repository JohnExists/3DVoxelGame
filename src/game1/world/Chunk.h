#ifndef CHUNK_H
#define CHUNK_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <chrono>

#include "../Random.h"
#include "../render/Texture.h"
#include "../render/Renderer.h"
#include "../render/Mesh.h"

#include "biome/Biome.h"
#include "biome/BiomeLoader.h"
#include "../render/CubeBuilder.h"
#include "FastNoise.h"
#include "Block.h"
#include "World.h"
#include "Decoration.h"
#include "World.h"

class BiomeLoader;
class Biome;
class BlockBuilder;
class Block;
class Renderer;
class World;
class Mesh;

class Chunk
{
public:
	static const int MAX_XZ = 16;
	static const int MAX_Y = 256;

	static const int PRIMARY_MESH = 0;
	static const int SECONDARY_MESH = 1;
	static const int TERTIARY_MESH = 2;

private:
	using Location_t = glm::vec3;
	using LocalLocation_t = glm::vec3;

	Mesh* primary = nullptr;
	Mesh* secondary = nullptr;
	Mesh* tertiary = nullptr;
	glm::vec2 position;
	Block* blocks;
	BiomeType* biomes;

	World* world = nullptr;
	BiomeLoader* loader = nullptr;
	FastNoiseLite* noise = nullptr;
	bool currentlyDrawing = false;

	Chunk* cacheFront = nullptr;
	Chunk* cacheBehind = nullptr;
	Chunk* cacheLeft = nullptr;
	Chunk* cacheRight = nullptr;


public:
	Chunk(const glm::vec2& position, World* world, BiomeLoader& loader, FastNoiseLite* noise);
	~Chunk();

	void draw(Renderer& renderer, int meshToDraw);
	void buildBlocks();
	bool isReady();
	bool isCurrentlyDrawing() const;
	void loadVBOs(int meshToLoad);

	Block& getBlockAt(Location_t& positon);
	void setBlockAt(Location_t& position, BlockType newBlock, bool replaceAir);
	void ghostSetBlockAt(Location_t position, BlockType newBlock);

	BiomeType getBiomeAt(int x, int z);
	void setBiomeAt(int x, int z, BiomeType newBiome);

	const glm::vec2& getPosition() const;
	Chunk* getCachedChunk(char cachedDirection) const;

	Location_t toWorldCoordinatesAt(int x, int y, int z);
	LocalLocation_t toChunkCoordinatesAt(int x, int y, int z);

	void loadDecoration(Decoration decoration, int x, int y, int z);

private:
	void cacheChunks();

	void loadMatrix(Renderer& renderer);
	void loadBlocks();

	BlockType getBlockTypeAt(int x, int y, int z);
	BlockType getBlockTypeAt(LocalLocation_t position);

	bool shouldRender(const LocalLocation_t& position, bool isBlockWater);

	std::uint8_t getFlags(int x, int y, int z);
	glm::vec3 getVertexPosition(const glm::vec3& blockPosition, const glm::vec3& positon);
	void buildBlockAt(int x, int y, int z);

	Block& getLocalBlockAt(int x, int y, int z) const;
	Block& getLocalBlockAt(LocalLocation_t position) const;


	bool isBlockWithinBounds(int x, int z);


};


#endif // !CHUNK_H