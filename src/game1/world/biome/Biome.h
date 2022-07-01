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
	int scaleBy = 0;
	int addBy = 0;
	int decorationCount;

	Placeables_t *decoration;

	BlockType topBlock = BlockType::AIR;
	BlockType centerBlocks = BlockType::AIR;
	BlockType bottomBlocks = BlockType::AIR;

public:
	Biome(int scaleBy, int addBy, BlockType top, BlockType center, BlockType bottom,
		  std::initializer_list<Placeables_t> decoration);

	BlockType get(int valueToGet);
	int obtainHeight(float noiseOutput);
	Decoration obtainDecoration();

	friend bool operator<(const Placeables_t &left, const Placeables_t &right);

private:
	bool shouldPlace(float percentage);
};

#endif // !BIOME_H
