#ifndef BIOME_H
#define BIOME_H

#include<FastNoise.h>

#include<array>
#include<vector>
#include<algorithm>
#include<map>

#include"../Block.h"
#include"../Decoration.h"
#include"../Chunk.h"
#include"../../Constants.h"
#include"BiomeLoader.h"

class Block;
class Chunk;
class World;
class BiomeLoader;

class Biome
{
public:
	using Placeables_t = std::pair<float, Decoration>;

	static const int TOP = 0;
	static const int CENTER = 1;
	static const int BOTTOM = 2;

private:
	int minHeight = 0;
	int altitude = 0;
	int decorationCount;

	Placeables_t *decoration;

	BlockType topBlock = BlockType::AIR;
	BlockType centerBlocks = BlockType::AIR;
	BlockType bottomBlocks = BlockType::AIR;

public:
	/**
	 * @brief Initializes a specific biome based on certain values
	 * 
	 * @param minHeight the minimum block height of that biome
	 * @param altitude The maximum block height of that biome
	 * @param top The block type that is in the highest point of the y location
	 * @param center The block type that is in the middle point of y location
	 * @param bottom The block type that is in the lowest point of y location
	 * @param decoration The decorating objects used for that biome
	 * 
	 */
	Biome(int minHeight, int altitude, BlockType top, BlockType center, BlockType bottom,
		  std::initializer_list<Placeables_t> decoration);

	/**
	 * @param valueToGet the value of the block height (TOP, CENTER, BOTTOM)
	 * 
	 * @returns Returns the block type at a y-location in the biome
	 */
	BlockType get(int valueToGet);
	/**
	 * @param noiseOutput The perlin noise value
	 * 
	 * @returns The height of an xz point given a noise input
	 */
	int obtainHeight(float noiseOutput);

	/**
	 * @returns The decorations  of the biome as a decoration object
	 */
	Decoration obtainDecoration();

	/**
	 * @brief Compares placeables by their float values
	 */
	friend bool operator<(const Placeables_t &left, const Placeables_t &right);

private:
	bool shouldPlace(float percentage);
};

#endif // !BIOME_H
