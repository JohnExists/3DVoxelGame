#include "BiomeLoader.h"

BiomeLoader::BiomeLoader(World* world, const FastNoiseLite& noise)
{
	this->noise = noise;
	this->world = world;
}

BlockType BiomeLoader::getBlockAt(int y, int topY, BiomeType biome)
{
	using enum BlockType;
	Biome& biomeData = getBiomeData(biome);

	if (y <= SEA_LEVEL && y > topY) 		return WATER;
	if (y <= SEA_LEVEL + 3 && y == topY)	return SAND;
	if (y == topY) 							return biomeData.get(Biome::TOP);
	if (y < topY && y > topY - 3)			return biomeData.get(Biome::CENTER);
	if (y <= topY - 3)						return biomeData.get(Biome::BOTTOM);
	return AIR;
}

void BiomeLoader::loadBiomes(Chunk* chunk)
{
	for (int x = 0; x < Chunk::MAX_XZ; x++)
	{
		for (int z = 0; z < Chunk::MAX_XZ; z++)
		{
			glm::vec3 worldPositon = chunk->toWorldCoordinatesAt(x, 0, z);

			BiomeType biome = getBiomeAt(worldPositon.x, worldPositon.z);
			chunk->setBiomeAt(x, z, biome);
		}
	}
}

void BiomeLoader::loadXZ(float x, float z, Chunk* chunk)
{
	BiomeType biome = chunk->getBiomeAt(x, z);
	Biome biomeData = getBiomeData(biome);

	int maxY = getYHeight(x, z, chunk->getPosition());

	if (maxY > SEA_LEVEL + 5)
		chunk->loadDecoration(biomeData.obtainDecoration(), x, maxY, z);


	for (int y = 0; y <= std::max(SEA_LEVEL, maxY); y++)
	{
		BlockType block = getBlockAt(y, maxY, biome);
		chunk->ghostSetBlockAt(glm::vec3(x, y, z), block);
	}
}

BiomeType BiomeLoader::getBiomeAt(float x, float z)
{
	const static int totalBiomes = static_cast<int>(BiomeType::TOTAL_BIOMES);

	x /= 6.f;
	z /= 6.f;

	float generatedNoise = abs(noise.getNoise(x, 0.0f, z));
	int biomeType = static_cast<int>(generatedNoise * totalBiomes);

	BiomeType result = static_cast<BiomeType>(glm::clamp(biomeType, 0, totalBiomes));

	return result;
}
//////////////////////////////////
/*		Private Functions		*/
//////////////////////////////////

Biome& BiomeLoader::getBiomeData(BiomeType biome)
{
	using enum BlockType;

	static Biome biomes[] = {
		// Ocean
		Biome(25, 5, STONE, STONE, STONE, { }),

		// Forest
		Biome(62, 23, GRASS, DIRT, STONE, {
			Placeables_t(1.5f, Decoration::TREE),
			Placeables_t(25.0f, Decoration::GRASS_BLADE),
		}),

		// Plains
		Biome(55, 12, GRASS, DIRT, STONE, {
			Placeables_t(0.075f, Decoration::TREE),
			Placeables_t(15.0f, Decoration::GRASS_BLADE),
		}),

		// Plains Mountain
		Biome(85, 25, GRASS, DIRT, STONE, { 
			Placeables_t(0.025f, Decoration::TREE),
			Placeables_t(45.0f, Decoration::GRASS_BLADE),
		}),

		// Desert
		Biome(60, 5, SAND, SAND, STONE, { }),

		// Desert Mountain
		Biome(75, 35, SAND, SAND, STONE, { }),
	
	};

	int selectedBiomeIndex = static_cast<int>(biome);
	return biomes[selectedBiomeIndex];
}

float BiomeLoader::calculateYHeightAt(glm::vec2 position)
{
	BiomeType biome = getBiomeAt(position.x, position.y);
	float noiseData = noise.getNoise(position.x, position.y);
	return calculateYHeightAt(biome, abs(noiseData));
}

float BiomeLoader::calculateYHeightAt(BiomeType biome, float perlinNoiseData)
{
	Biome& biomeData = getBiomeData(biome);
	float result = biomeData.obtainHeight(perlinNoiseData);

	return result;
}

int BiomeLoader::getYHeight(int worldX, int worldZ)
{
	int chunkX = floor(worldX / Chunk::MAX_XZ);	
	int chunkZ = floor(worldZ / Chunk::MAX_XZ);	
	int x = worldX - (chunkX * (Chunk::MAX_XZ - 1));
	int z = worldZ - (chunkZ * (Chunk::MAX_XZ - 1));

	return getYHeight(x, z, glm::vec2(chunkX, chunkZ));
}

int BiomeLoader::getYHeight(int x, int z, glm::vec2 chunkPosition)
{
	glm::vec3 a(chunkPosition.x * Chunk::MAX_XZ + 1, 0, chunkPosition.y * Chunk::MAX_XZ + 1);
	static const float w = 15.0f;
	glm::vec2 b[] = {
		glm::vec2(a.x, a.z),
		glm::vec2(a.x + w, a.z),
		glm::vec2(a.x, a.z + w),
		glm::vec2(a.x + w, a.z + w),
	};

	int q11 = calculateYHeightAt(b[0]);
	int q21 = calculateYHeightAt(b[1]);
	int q12 = calculateYHeightAt(b[2]);
	int q22 = calculateYHeightAt(b[3]);

	if (x == 0 && z == 0) 	return q11;
	if (x == 15 && z == 0) 	return q21;
	if (x == 0 && z == 15)	return q12;
	if (x == 15 && z == 15)	return q22;

	return biLerp(q11, q12, q21, q22, 0, 15, 0, 15, x, z);
}

float BiomeLoader::biLerp(float q11, float q12, float q21, float q22, float x1, float x2, float y1, float y2, float x, float y)
{
	float x2x1, y2y1, x2x, y2y, yy1, xx1;
	x2x1 = x2 - x1;
	y2y1 = y2 - y1;
	x2x = x2 - x;
	y2y = y2 - y;
	yy1 = y - y1;
	xx1 = x - x1;
	return 1.0 / (x2x1 * y2y1) * (q11 * x2x * y2y + q21 * xx1 * y2y + q12 * x2x * yy1 + q22 * xx1 * yy1);
}