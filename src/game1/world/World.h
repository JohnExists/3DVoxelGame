#ifndef WORLD_H
#define WORLD_H

#include<glm/glm.hpp>

#include<map>
#include<cmath>
#include<string>
#include<queue>
#include<deque>
#include<memory>
#include<thread>

#include"../render/Renderer.h"
#include"../render/Shader.h"
#include"../Camera.h"

#include"biome/Biome.h"
#include"biome/BiomeLoader.h"
#include"Chunk.h"
#include"FastNoise.h"
#include"Block.h"

class BiomeLoader;
class Chunk;
class Camera;
class Block;
class Renderer;
class BiomeLoader;

class World
{
public:
	static std::string vectorToString(const glm::vec2& vec);

private:
	const int RENDER_DISTANCE = 10;

	using ChunkList_t = std::map<std::string, Chunk*>;
	using Location_t = glm::vec3;
	using ChunkLocation_t = glm::vec2;

	bool currentlyDrawing = false;

	ChunkList_t chunks;
	std::deque<ChunkLocation_t> chunksBuildQueue;
	std::deque<Chunk*> chunksMeshQueue;
	FastNoiseLite* noise;
	BiomeLoader* loader;
	Texture* texture;

public:
	World(int seed);

	/**
	*
	* Returns a pointer to a chunk at a certain position
	* Return nullptr if no such chunk exists
	* 
	* @param position
	* Position that the chunk is located at
	* 
	*/
	Chunk* getChunkAt(const ChunkLocation_t& position);
	Chunk* getChunkAtWorld(const Location_t& position);
	Block* getBlockAt(Location_t position);

	void draw(Renderer& renderer);
	void updateChunks(Camera* camera);
	void addToQueue(Chunk* chunk);

	void updateChunksBuilds(Camera* camera, int task);
	void updateChunksMeshing();

	void castRay(Camera& camera, bool shouldBreak);
	void breakBlockAt(Location_t position);
	void placeBlockAt(Location_t position, BlockType type);

	const Texture* getTexture() const;
private:
	void unloadFarChunks(const ChunkLocation_t playerChunkLocation);
	void loadNearbyChunks(const ChunkLocation_t& nearby, bool firstTime);
	void loadChunk(const ChunkLocation_t& vectorPosition);
	World::Location_t getBlockSide(Location_t rayLanding);
};

#endif // !WORLD_H



