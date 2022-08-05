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
#include"../render/Frustum.h"

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

	static const int RENDER_DISTANCE = 12;
private:

	using ChunkPtr_t = std::unique_ptr<Chunk>;
	using ChunkList_t = std::map<std::string, std::unique_ptr<Chunk>>;
	using Location_t = glm::vec3;
	using ChunkLocation_t = glm::vec2;

	ChunkList_t chunks;
	FastNoiseLite* noise;
	BiomeLoader* loader;
	Texture* texture;

public:
	World(int seed);

	/**
	* @returns A pointer to the chunk at a certain position or 
	* nullptr if no chunk exists
	* 
	* @param position
	* Position that the chunk is located at based off chunk coordinates
	* 
	*/
	Chunk* getChunkAt(const ChunkLocation_t& position);

	/**
	* @returns A pointer to the chunk at a certain position or 
	* nullptr if no chunk exists
	* 
	* @param position
	* Position that the chunk is located at based off world coordinates
	* 
	*/
	Chunk* getChunkAtWorld(const Location_t& position);
	Block* getBlockAt(Location_t position);
	glm::vec2 getChunkPositionAt(Location_t postion);

	void draw(Renderer& renderer, Frustum& frustum);
	void updateChunks(Camera* camera);
	void addToQueue(Chunk* chunk);

	void updateChunksBuilds(Camera* camera, int task);

	void castRay(Camera& camera, bool shouldBreak);
	void breakBlockAt(Location_t position);
	void placeBlockAt(Location_t position, BlockType type);

	const Texture* getTexture() const;
private:
	void unloadFarChunks(const ChunkLocation_t playerChunkLocation);
	void loadNearbyChunks(const ChunkLocation_t& nearby, Camera* camera);
	void preloadChunk(const ChunkLocation_t& vectorPosition, Camera* camera);
	World::Location_t getBlockSide(Location_t rayLanding);
};

#endif // !WORLD_H



