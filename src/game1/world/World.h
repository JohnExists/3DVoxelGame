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
#include"../Settings.h"
#include"../GameState.h"

class BiomeLoader;
class Chunk;
class Camera;
class Block;
class Renderer;
class BiomeLoader;
class GameState;

class World
{
public:
	static std::string vectorToString(const glm::vec2& vec);

	static const int RENDER_DISTANCE = 12;
private:

	using ChunkPtr_t = std::unique_ptr<Chunk>;
	using ChunkList_t = std::map<std::string, std::unique_ptr<Chunk>>;
	
	ChunkList_t chunks;
	std::queue<std::string> garbageChunks;
	FastNoiseLite* noise;
	BiomeLoader* loader;
	Texture* texture;
	GameState* gameState;

public:
	World(GameState* gameState, int seed);

	/**
	* @returns A pointer to the chunk at a certain position or 
	* nullptr if no chunk exists
	* 
	* @param position
	* Position that the chunk is located at based off chunk coordinates
	* 
	*/
	Chunk* getChunkAt(const game::ChunkLocation_t& position);

	/**
	* @returns A pointer to the chunk at a certain position or 
	* nullptr if no chunk exists
	* 
	* @param position
	* Position that the chunk is located at based off world coordinates
	* 
	*/
	Chunk* getChunkAtWorld(const game::Location_t& position);
	Block* getBlockAt(game::Location_t position);
	glm::vec2 getChunkPositionAt(game::Location_t postion);

	void draw(Renderer& renderer, Frustum& frustum);
	void updateChunks(Camera* camera);
	void addToQueue(Chunk* chunk);

	void updateChunksBuilds(Camera* camera, int task);

	void clearGarabage();

	void breakBlockAt(game::Location_t position);
	void placeBlockAt(game::Location_t position, BlockType type);

	const Texture* getTexture() const;
	GameState& getGameState();

	void preloadChunk(const game::ChunkLocation_t& vectorPosition, Camera* camera);
private:
	void unloadFarChunks(const game::ChunkLocation_t playerChunkLocation);
	void loadNearbyChunks(const game::ChunkLocation_t& nearby, Camera* camera);
};

#endif // !WORLD_H



