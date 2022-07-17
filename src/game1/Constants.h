#ifndef CONSTANTS_H
#define CONSTANTS_H

enum class BlockType
{
	AIR = 0,
	GRASS,
	DIRT,
	STONE,
	WOOD,
	LEAVES,
	WATER,
	SAND,
	GRASS_BLADES,
};

enum class BiomeType
{
	OCEAN,
	FOREST,
	PLAINS,
	PLAINS_MOUNTAIN,
	DESERT,
	DESERT_MOUNTAIN,
	TOTAL_BIOMES
};

enum class Decoration
{
	TREE,
	GRASS_BLADE,
	NOTHING
};

#define GET_NEARBY_BLOCKS(x, y, z) {\
	{ x,		y,		z - 1 }, \
	{ x,		y,		z + 1 }, \
	{ x - 1,	y,		z	  }, \
	{ x + 1,	y,		z	  }, \
	{ x,		y + 1,	z	  }, \
	{ x,		y - 1,	z	  }, \
};

#define PRINT_VECTOR(b) std::cout << "X: " << b.x << " Y: " << b.y << " Z: " << b.z << '\n';


#endif // !CONSTANTS_H
