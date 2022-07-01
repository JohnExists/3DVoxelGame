#include "Chunk.h"

//////////////////////////////////
/*		Public Functions		*/
//////////////////////////////////

Chunk::Chunk(const glm::vec2& position, World* world, BiomeLoader& loader, FastNoiseLite* noise)
	: loader(&loader), position(position), world(world), noise(noise)
{
	blocks = new Block[MAX_XZ * MAX_Y * MAX_XZ]();
	biomes = new BiomeType[MAX_XZ * MAX_XZ];

	this->loader->loadBiomes(this);
	loadBlocks();
}

Chunk::~Chunk()
{
	delete[] blocks;
	delete[] biomes;
	delete primary;
	delete secondary;
	delete tertiary;
}


void Chunk::draw(Renderer& renderer, int meshToDraw)
{
	currentlyDrawing = true;
	loadMatrix(renderer);

	if (meshToDraw == PRIMARY_MESH && primary != nullptr)
	{
		glEnable(GL_CULL_FACE);
		primary->draw();
	}
	if (meshToDraw == TERTIARY_MESH && tertiary != nullptr)
	{
		glDisable(GL_CULL_FACE);
		tertiary->draw();
	}
	if (meshToDraw == SECONDARY_MESH && secondary != nullptr)
	{
		glEnable(GL_CULL_FACE);
		secondary->draw();
	}
	currentlyDrawing = false;
}

void Chunk::buildBlocks()
{
	cacheChunks();

	delete primary;
	delete secondary;
	delete tertiary;

	primary = new Mesh(world->getTexture());
	secondary = new Mesh(world->getTexture());
	tertiary = new Mesh(world->getTexture());


	for (int x = 0; x < MAX_XZ; x++)
	{
		for (int z = 0; z < MAX_XZ; z++)
		{
			for (int y = 0; y < MAX_Y; y++)
			{
				Block& block = getLocalBlockAt(x, y, z);

				if (block.isAir()) continue;
				buildBlockAt(x, y, z);
			}
		}

	}

	while (true)
	{
		if(!isCurrentlyDrawing())
		{
			primary->clearVertexObject();
			secondary->clearVertexObject();
			tertiary->clearVertexObject();
			break;
		}
	}
	
}

bool Chunk::isReady()
{
	cacheChunks();

	return !((cacheFront == nullptr) ||
		(cacheBehind == nullptr) ||
		(cacheLeft == nullptr) ||
		(cacheRight == nullptr));
}

bool Chunk::isCurrentlyDrawing() const 
{
	return currentlyDrawing;
}

void Chunk::loadVBOs(int meshToLoad)
{
	switch (meshToLoad)
	{
	case PRIMARY_MESH:
		if(!primary->isLoaded()) primary->loadVertexObject();
		break;
	case SECONDARY_MESH:
		if(!secondary->isLoaded()) secondary->loadVertexObject();
		break;
	case TERTIARY_MESH:
		if(!tertiary->isLoaded()) tertiary->loadVertexObject();
		break;
	}
}

void Chunk::setBiomeAt(int x, int z, BiomeType newBiome)
{
	biomes[MAX_XZ * x + z] = newBiome;
}

BiomeType Chunk::getBiomeAt(int x, int z)
{
	return biomes[MAX_XZ * x + z];
}

Block& Chunk::getBlockAt(Location_t& blockPositon)
{
	return getLocalBlockAt(
		blockPositon.x - (this->position.x * MAX_XZ),
		blockPositon.y,
		blockPositon.z - (this->position.y * MAX_XZ)
	);

}

void Chunk::setBlockAt(Location_t& position, BlockType newBlock, bool replaceNonAirBlocks)
{
	Block& block = getBlockAt(position);
	if (!replaceNonAirBlocks && (block != BlockType::AIR)) return;
	LocalLocation_t local = toChunkCoordinatesAt(position.x, position.y, position.z);
	block.setType(newBlock);
	world->addToQueue(this);
	if (local.x < 1)			world->addToQueue(cacheLeft);
	if (local.x >= MAX_XZ - 1)	world->addToQueue(cacheRight);
	if (local.z < 1)			world->addToQueue(cacheBehind);
	if (local.z >= MAX_XZ - 1)	world->addToQueue(cacheFront);
}

const glm::vec2& Chunk::getPosition() const
{
	return position;
}

Chunk* Chunk::getCachedChunk(char cachedDirection) const
{
	switch (cachedDirection)
	{
	case 'F': return cacheFront;
	case 'B': return cacheBehind;
	case 'L': return cacheLeft;
	case 'R': return cacheRight;
	default: return nullptr;
	}
}

void Chunk::ghostSetBlockAt(Location_t position, BlockType newBlock)
{
	Block& block = getLocalBlockAt(position);
	block = newBlock;
}

//////////////////////////////////
/*		Private Functions		*/
//////////////////////////////////

void Chunk::cacheChunks()
{
	cacheLeft = world->getChunkAt(position + glm::vec2(-1, 0)); // LEFT
	cacheRight = world->getChunkAt(position + glm::vec2(1, 0));  // RIGHT
	cacheBehind = world->getChunkAt(position + glm::vec2(0, -1)); // BEHIND
	cacheFront = world->getChunkAt(position + glm::vec2(0, 1));  // FRONT

}

void Chunk::loadMatrix(Renderer& renderer)
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(position.x * MAX_XZ, 0, position.y * MAX_XZ));

	if (primary != nullptr) primary->setModelMatrix(model);
	if (secondary != nullptr) secondary->setModelMatrix(model);
	if (tertiary != nullptr) tertiary->setModelMatrix(model);

	Shader& shader = renderer.getShader(ShaderType::DEFAULT_SHADER);
	shader.setMatrix("model", model);
}

void Chunk::loadBlocks()
{

	for (int x = 0; x < MAX_XZ; x++)
	{
		for (int z = 0; z < MAX_XZ; z++)
		{
			loader->loadXZ(x, z, this);
		}
	}

}

void Chunk::loadDecoration(Decoration decoration, int x, int y, int z)
{
	if(decoration == Decoration::NOTHING) return;
	for (const auto& [block, position] : decor::load(decoration))
	{
		LocalLocation_t decorLocation(x + position.x, y + position.y, z + position.z);
		if (!isBlockWithinBounds(decorLocation.x, decorLocation.z)) continue;
		getLocalBlockAt(decorLocation).setType(block);
	}
}

Block& Chunk::getLocalBlockAt(int x, int y, int z) const
{
	return blocks[x + MAX_XZ * (y + MAX_Y * z)];
}

Block& Chunk::getLocalBlockAt(LocalLocation_t vector) const
{
	return getLocalBlockAt(vector.x, vector.y, vector.z);
}

BlockType Chunk::getBlockTypeAt(int x, int y, int z)
{
	if (y < 0) return BlockType::AIR;

	if (!isBlockWithinBounds(x, z))
	{
		Location_t worldLocation = toWorldCoordinatesAt(x, y, z);
		Chunk* storedChunk = nullptr;
		if (x < 0) storedChunk = cacheLeft;
		if (x > MAX_XZ - 1) storedChunk = cacheRight;
		if (z < 0) storedChunk = cacheBehind;
		if (z > MAX_XZ - 1) storedChunk = cacheFront;

		if (storedChunk != nullptr) 
			return storedChunk->getBlockAt(worldLocation).getType();

		return loader->getBlockAt(
			y, 
			loader->calculateYHeightAt(glm::vec2(worldLocation.x, worldLocation.z)),
			loader->getBiomeAt(worldLocation.x, worldLocation.z)
		);
	}
	return getLocalBlockAt(x, y, z).getType();

}

BlockType Chunk::getBlockTypeAt(LocalLocation_t vector)
{
	return getBlockTypeAt(vector.x, vector.y, vector.z);
}

bool Chunk::shouldRender(const LocalLocation_t& pos, bool isBlockWater = false)
{
	if (pos.y < 0) return false;
	bool airCheck = getBlockTypeAt(pos) == BlockType::AIR ||
		getBlockTypeAt(pos) == BlockType::GRASS_BLADES;

	if (!isBlockWater) return airCheck;
	return getBlockTypeAt(pos) == BlockType::WATER || airCheck;
}

std::uint8_t Chunk::getFlags(int x, int y, int z)
{
	glm::vec3 positions[] = GET_NEARBY_BLOCKS(x, y, z);
	BlockType blockType = getBlockTypeAt(x, y, z);
	std::uint8_t flags{};

	for (int i = 0; i < 6; i++)
	{
		BlockType nearType = getBlockTypeAt(positions[i]);
		if (blockType == BlockType::WATER)
		{
			if (nearType == BlockType::AIR && nearType != BlockType::WATER)
				flags |= 1 << i;
		}
		else
		{
			if (shouldRender(positions[i], blockType != BlockType::WATER))
				flags |= 1 << i;
		}
	}

	return flags;
}

glm::vec3 Chunk::getVertexPosition(const glm::vec3& blockPosition,
	const glm::vec3& verticesPositon)
{
	glm::fmat4 translationMatrix = glm::translate(glm::fmat4(1.0f), glm::vec3(
		static_cast<float>(blockPosition.x),
		static_cast<float>(blockPosition.y),
		static_cast<float>(blockPosition.z))
	);

	glm::vec4 vec = translationMatrix *
		glm::vec4(verticesPositon.x, verticesPositon.y, verticesPositon.z, 1.0f);

	return glm::vec3(vec.x, vec.y, vec.z);
}

void Chunk::buildBlockAt(int x, int y, int z)
{
	Block& currentBlock = getLocalBlockAt(x, y, z);

	if (currentBlock == BlockType::AIR) return;
	if (!isBlockWithinBounds(x, z)) return;

	std::vector<Quad> blockQuads;
	if (currentBlock != BlockType::GRASS_BLADES)
		CubeBuilder::loadVertices(
			getFlags(x, y, z),
			blockQuads,
			BlockBuilder::genTexCoords(currentBlock.getType())
		);
	else
		CubeBuilder::loadGrassVertices(
			blockQuads,
			BlockBuilder::genTexCoords(BlockType::GRASS_BLADES)[0]
		);

	for (auto& quads : blockQuads)
	{
		for (auto& v : quads.vertices)
		{
			v.position = getVertexPosition(glm::vec3(x, y, z), v.position);

		}
		int render = currentBlock.getRenderType();
		if (render == PRIMARY_MESH) primary->getQuads().push_back(quads);
		if (render == SECONDARY_MESH) secondary->getQuads().push_back(quads);
		if (render == TERTIARY_MESH) tertiary->getQuads().push_back(quads);
	}
}

glm::vec3 Chunk::toWorldCoordinatesAt(int x, int y, int z)
{
	return glm::vec3(position.x * MAX_XZ + x, y, position.y * MAX_XZ + z);
}

glm::vec3 Chunk::toChunkCoordinatesAt(int x, int y, int z)
{
	return glm::vec3(
		x - (getPosition().x * MAX_XZ) - 1,
		y,
		z - (getPosition().y * MAX_XZ) - 1
	);
}

bool Chunk::isBlockWithinBounds(int x, int z)
{
	return !(x > MAX_XZ - 1 || x < 0
		|| z > MAX_XZ - 1 || z < 0);
}
