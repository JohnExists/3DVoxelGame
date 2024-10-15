#ifndef BIOMELOADER_H
#define BIOMELOADER_H

#include"../World.h"
#include"../Chunk.h"

class Biome;
class World;
class Chunk;

class BiomeLoader
{
private:
	const int SEA_LEVEL = 50;
	using Placeables_t = std::pair<float, Decoration>;

	FastNoiseLite noise;
	World* world;
	

public:
	/**
	 * @brief Initializes the biome loader for the world 
	 * 
	 * @param world The world that the biome loader is in
	 * @param noise The random number generator that is used to generate noise
	 */
	BiomeLoader(World* world, const FastNoiseLite& noise);

	/**
	 * @brief Loads all the biomes in a speific chunk
	 * 
	 * @param chunk The chunk that the biome will be loaded
	 */
	void loadBiomes(Chunk* chunk);
	/**
	 * @brief Generates all the biome blocks at a speific location
	 * 
	 * @param xz The x and z location relative to the chunk
	 * @param chunk The chunk that the biome will be generated at
	 */
	void loadXZ(float x, float z, Chunk* chunk);

	/**
	 * @brief Generates the blocks at a certain y-level
	 * 
	 * @param y The y level where the block will be generated
	 * @param yHeight the maximum y height of that xz location
	 * @param biome The biome of that location
	 * 
	 * @returns The blocktype at that position
	 */
	BlockType getBlockAt(int y, int yHeight, BiomeType biome);

	/**
	 * @param xz The world coordinates to get the biome at
	 * 
	 * @returns The biome type at a certain location
	 */
	BiomeType getBiomeAt(float x, float z);

	/**
	 * @param worldXZ The x and z coordinates for the maximum y
	 * 
	 * @returns The maximum y-height that blocks will be placed at that location
	 */
	int getYHeight(int worldX, int worldZ);
	/**
	 * @param xz The x and z coordinates for the maximum y
	 * @param chunkPosition The chunk location relative to the world
	 * 
	 * @returns The maximum y-height that blocks will be placed at that location
	 */
	int getYHeight(int x, int z, glm::vec2 chunkPosition);
private:
	Biome& getBiomeData(BiomeType biome);

	float calculateYHeightAt(glm::vec2 position);
	float calculateYHeightAt(BiomeType biome, float perlinNoiseData);

	float biLerp(float q11, float q12, float q21, float q22, float x1, float x2, float y1, float y2, float x, float y);

};

#endif // BIOMELOADER_H
