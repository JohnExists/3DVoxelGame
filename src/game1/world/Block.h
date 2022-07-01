#ifndef BLOCK_H
#define BLOCK_H

#include<glm/glm.hpp>

#include<array>

#include"../render/TextureAtlas.h"
#include"../Constants.h"
#include"biome/Biome.h"

class BiomeLoader;

class Block
{
public:

private:
	unsigned char type;
public:
	Block();
	Block(BlockType type);

	BlockType getType() const;
	void setType(BlockType type);

	bool isTransparent() const;
	bool isFullBlock() const;
	bool isAir() const;
	bool isSolid() const;

	int getRenderType();

	friend bool operator==(const Block& left, const BlockType right);
	friend bool operator!=(const Block& left, const BlockType right);

private:

};

class BlockBuilder
{

public:

	static tex::AtlasCollection_t genTexCoords(const Block& block);

private:
	static float toTex(float coord);
};

#endif // !BLOCK_H


