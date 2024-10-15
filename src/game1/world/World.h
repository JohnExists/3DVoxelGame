#ifndef WORLD_H
#define WORLD_H

#include<glm/glm.hpp>

#include<map>
#include<cmath>
#include<string>
#include<queue>
#include<deque>
#include<memory>

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

	static const int RENDER_DISTANCE = 7;
private:
	// Useful type aliases
	using ChunkPtr_t = std::unique_ptr<Chunk>;
	using ChunkList_t = std::map<std::string, std::unique_ptr<Chunk>>;
	
	// All chunkss stored
	ChunkList_t chunks;
	// Chunks that will be deleted
	std::queue<std::string> garbageChunks;
	// Used for random terrain generation
	FastNoiseLite* noise;
	// Used for determining which biome is at a specific position
	BiomeLoader* loader;
	// The textures that will be used for the world
	Texture* texture;
	// The stored GameState object
	GameState* gameState;

public:
	/**
	 * @brief Initializes the world to its default values
	 * 
	 * @param gameState Stores the current active game state in this world
	 * @param seed The seed used for the random terrain of this world
	 */
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
	* @param position X, Y, Z Position that the chunk is located at based off world coordinates
	* 
	* @returns A pointer to the chunk at a certain position or 
	* nullptr if no chunk exists
	* 
	*/
	Chunk* getChunkAtWorld(const game::Location_t& position);
	
	/**
	 * @param position The position of the block that is getting located
	 * 
	 * @returns Pointer to the block at that speicific block at that position
	 * 
	 */
	Block* getBlockAt(game::Location_t position);
	/**
	 * @brief Obtains the chunk coordinates relative to the world
	 * 
	 * @param position X, Y, Z world coordinates for that chunk
	 * 
	 * @returns The x, y chunk coordinates at that x, y, z postion
	 */
	glm::vec2 getChunkPositionAt(game::Location_t postion);
	/**
	 * @brief Renders the world onto the screen
	 * 
	 * @param renderer The renderer object used for drawing the world
	 * @param frustum The frustum of the player used for clipping objects
	 * out of view
	 * 
	 */
	void draw(Renderer& renderer, Frustum& frustum);
	/**
	 * @brief Builds the blocks for a chunk
	 * 
	 * @param chunk The chunk that will have its block built
	 */
	void addToQueue(Chunk* chunk);

	/**
	 * @brief Either builds all world chunks or unloads all chunks
	 * 
	 * @param camera The players camera that will be updated
	 * @param task 0 = Builds Nearby Chunks, 1 = Destroy far away chunks
	 */
	void updateChunksBuilds(Camera* camera, int task);
	/**
	 * Clear all the deleted/unloaded chunks of the world
	 */
	void clearGarabage();
	/**
	 * @brief Destroy the block at a certain x, y, z coordinate
	 * 
	 * @param position The position of that block
	 * @returns The block broken at that specific position
	 */
	BlockType breakBlockAt(game::Location_t position);
	/**
	 * @brief Places a block at a certain position relative to the world
	 * 
	 * @param position The x, y, z of the new block
	 * @param type The type of block that will be placed
	 * 
	 */
	void placeBlockAt(game::Location_t position, BlockType type);

	/**
	 * @returns Pointer to the texture object that contains the entire texture atlas
	 */
	const Texture* getTexture() const;
	/**
	 * @returns Pointer to the game state that this game is in
	 */
	GameState& getGameState();

	/**
	 * @brief Loads a chunk at a specific location, primarily used
	 * for first loading into the world
	 * 
	 * @param vectorPosition The location of that new chunk
	 * @param camera The camera of the player that needs to be updated
	 */
	void preloadChunk(const game::ChunkLocation_t& vectorPosition, Camera* camera);

	/**
	 * @brief Checks if any object collides with the surface of the world
	 * 
	 * @param object The block to check against
	 * @returns Whether that object collides or not
	 */
	bool collidesWithSurface(AABB object);
	/**
	 * @brief Deletes any chunks that are far away from the player
	 * 
	 * @param playerChunkLocation The location of the chunk that the player is at
	 */
	void unloadFarChunks(const game::ChunkLocation_t playerChunkLocation);
	/**
	 * @brief Loads any new chunks that are close to a specific chunk location
	 * 
	 * @param nearby The location of the chunk that the player is at
	 * @param camera The players camera
	 */
	void loadNearbyChunks(const game::ChunkLocation_t& nearby, Camera* camera);
};

#endif // !WORLD_H



