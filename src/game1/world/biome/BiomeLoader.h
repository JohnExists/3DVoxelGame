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
	BiomeLoader(World* world, const FastNoiseLite& noise);

	void loadBiomes(Chunk* chunk);
	void loadXZ(float x, float z, Chunk* chunk);

	BlockType getBlockAt(int y, int yHeight, BiomeType biome);
	BiomeType getBiomeAt(float x, float z);

	float calculateYHeightAt(glm::vec2 position);
	float calculateYHeightAt(BiomeType biome, float perlinNoiseData);
private:
	Biome& getBiomeData(BiomeType biome);

	int getYHeight(int x, int z, Chunk* blockChunk);

	float biLerp(float q11, float q12, float q21, float q22, float x1, float x2, float y1, float y2, float x, float y);

};

#endif // BIOMELOADER_H
