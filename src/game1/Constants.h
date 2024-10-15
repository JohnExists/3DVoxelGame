#ifndef CONSTANTS_H
#define CONSTANTS_H

// All the enum values for the blocks present in the game
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

// All the enum values for the biomes present in the game
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

// All the enum values for the "decorations" (items placed in the world)
enum class Decoration
{
	TREE,
	GRASS_BLADE,
	NOTHING
};

// Gets all the nearby blocks as a massive list of x, y, z vectors
#define GET_NEARBY_BLOCKS(x, y, z) {\
	{ x,		y,		z - 1 }, \
	{ x,		y,		z + 1 }, \
	{ x - 1,	y,		z	  }, \
	{ x + 1,	y,		z	  }, \
	{ x,		y + 1,	z	  }, \
	{ x,		y - 1,	z	  }, \
};

// Prints a 3d vector to the console
#define PRINT_VECTOR(b) std::cout << "X: " << b.x << " Y: " << b.y << " Z: " << b.z << '\n';


#endif // !CONSTANTS_H
