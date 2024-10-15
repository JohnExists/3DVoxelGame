#ifndef BLOCK_H
#define BLOCK_H

#include<glm/glm.hpp>

#include<array>

#include"../render/TextureAtlas.h"
#include"../Constants.h"
#include"../AABB.h"
#include"biome/Biome.h"

class BiomeLoader;

class Block
{
public:

private:
	// The block stored
	unsigned char type;
public:
	/**
	 * @brief Initializes the block
	 */
	Block();
	Block(BlockType type);

	BlockType getType() const;
	void setType(BlockType type);

	/**
	 * @returns True when the block is see-through
	 */
	bool isTransparent() const;
	/**
	 * @returns True if the full size of a block (ex. True if stone, false if grass blade)
	 */
	bool isFullBlock() const;
	/**
	 * @returns True if the block has the type AIR
	 */
	bool isAir() const;
	/**
	 * @returns True if the block can be walked through (ex. True if grass blade, false if stone)
	 */
	bool isSolid() const;

	/**
	 * @returns The way that the block should be rendered
	 * (ie water is rendered differently than grass)
	 */
	int getRenderType();

	// Used as operators the each block
	friend bool operator==(const Block& left, const BlockType right);
	friend bool operator!=(const Block& left, const BlockType right);

private:

};

class BlockBuilder
{

public:
	/**
	 * @brief Generates vertices of the texture for each
	 */
	static tex::AtlasCollection_t genTexCoords(const Block& block);
	static tex::AtlasCollection_t genTexCoords(const BlockType block);

private:
	static float toTex(float coord);
};

#endif // !BLOCK_H


