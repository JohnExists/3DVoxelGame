#include "World.h"

//////////////////////////////////
/*		Public Functions		*/
//////////////////////////////////

Block::Block()
{
	setType(BlockType::AIR);
}

Block::Block(BlockType type)
{
	setType(type);
}

BlockType Block::getType() const
{
   	return static_cast<BlockType>(type);
}

void Block::setType(BlockType type)
{
	char newType = static_cast<unsigned char>(type);
	this->type = newType;
}

bool Block::isTransparent() const
{
	return (getType() == BlockType::WATER);
}

bool Block::isFullBlock() const
{
	return getType() == BlockType::GRASS_BLADES;
}

bool Block::isAir() const
{
	return getType() == BlockType::AIR;
}

bool Block::isSolid() const {
	return (getType() != BlockType::WATER) &&
		(getType() != BlockType::WATER);
}

int Block::getRenderType()
{
	if (getType() == BlockType::GRASS_BLADES) return 1;
	if (isTransparent()) return 1;
	else return 0;
}

bool operator==(const Block& left, const BlockType right)
{
	return left.getType() == right;
}

bool operator!=(const Block& left, const BlockType right)
{
	return left.getType() != right;
}

tex::AtlasCollection_t BlockBuilder::genTexCoords(const Block& block)
{
	static tex::AtlasCollection_t blockAtlas[] = {
		tex::load("grass.json"), 
		tex::load("dirt.json"),
		tex::load("stone.json"), 
		tex::load("wood.json"),
		tex::load("leaves.json"), 
		tex::load("water.json"),
		tex::load("sand.json"), 
		tex::load("grass_blade.json")
	};

	assert(block != BlockType::AIR && "Cannot obtain AIR texture coordinates");

	int blockIndex = static_cast<int>(block.getType()) - 1;
	return blockAtlas[blockIndex];
}

//////////////////////////////////
/*		Private Functions		*/
//////////////////////////////////

float BlockBuilder::toTex(float coord)
{
	return coord/ 8.0f;
}
