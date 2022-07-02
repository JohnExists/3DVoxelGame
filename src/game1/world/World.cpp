#include "World.h"

std::string World::vectorToString(const glm::vec2& vec)
{
	return std::to_string(vec.x) + ':' +
		std::to_string(vec.y);
}

World::World(int seed)
{
	noise = new FastNoiseLite(seed);
	loader = new BiomeLoader(this, *noise);
	noise->setNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);
	noise->SetFractalOctaves(16);

	this->texture = new Texture("../res/atlas.png");

	texture->setScalingFilter(GL_NEAREST, GL_NEAREST);
	texture->setWrapAround(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

	loadNearbyChunks(ChunkLocation_t(0.0f, 0.0f), true);
}

Chunk* World::getChunkAt(const ChunkLocation_t& vectorPosition)
{ 
	std::string position = vectorToString(vectorPosition);
	Chunk* chunk = nullptr;
	if (chunks.count(position)) chunk = chunks.at(position);

	return chunk;
}

Chunk* World::getChunkAtWorld(const Location_t& position)
{
	ChunkLocation_t chunkPosition = glm::vec2(
		floor(position.x / Chunk::MAX_XZ),
		floor(position.z / Chunk::MAX_XZ)
	);

	return getChunkAt(chunkPosition);
}

Block* World::getBlockAt(Location_t position)
{
	Chunk* chunk = getChunkAtWorld(position);
	if (chunk == nullptr) return nullptr;
	return &chunk->getBlockAt(position);
}


void World::draw(Renderer& renderer)
{
	currentlyDrawing = true;
	texture->useSlot(renderer.getShader(ShaderType::DEFAULT_SHADER), "texture1", 0);

	for (auto& [position, chunk] : chunks)
	{
		if(chunk != nullptr) chunk->draw(renderer, Chunk::PRIMARY_MESH);
	}
	for (auto& [position, chunk] : chunks)
	{
		if(chunk != nullptr) chunk->draw(renderer, Chunk::SECONDARY_MESH);
		if(chunk != nullptr) chunk->draw(renderer, Chunk::TERTIARY_MESH);
	}
	currentlyDrawing = false;
}

void World::addToQueue(Chunk* chunk)
{
	chunk->buildBlocks();
}

void World::updateChunksBuilds(Camera* camera, int task) 
{
	static ChunkLocation_t playerChunk = glm::vec2(0.0f, 0.0f);
	Location_t playerLocation = camera->getPosition();

	if(playerChunk != getChunkAtWorld(playerLocation)->getPosition())
	{
		if(task == 0) loadNearbyChunks(playerChunk, true);
		if(task == 1) unloadFarChunks(playerChunk);
	}
	
	playerChunk = getChunkAtWorld(playerLocation)->getPosition();

}

void World::updateChunksMeshing()
{
	if (!chunksMeshQueue.empty())
	{
		Chunk* front = chunksMeshQueue.front();
		if (front != nullptr)
		{
			while (true)
			{
				if(!front->isCurrentlyDrawing())
				{
					front->buildBlocks();
					chunksMeshQueue.pop_front();
					break;
				}
			}
		}
	}
}

void World::breakBlockAt(Location_t position)
{
	Chunk* chunk = getChunkAtWorld(position);
	chunk->setBlockAt(position, BlockType::AIR, true);

}

void World::placeBlockAt(Location_t position, BlockType type)
{
	Chunk* chunk = getChunkAtWorld(position);
	chunk->setBlockAt(position, type, false);
}

void World::castRay(Camera& camera, bool shouldBreak)
{
	static const float PLAYER_REACH = 6.0f;

	Location_t castFrom = camera.getPosition();
	glm::vec3 castTo = camera.getDirectionVector();

	for (float rayLength = 0; rayLength <= PLAYER_REACH; rayLength += 0.01)
	{
		Location_t rayLocation = castFrom + (castTo * rayLength);
		Block* block = getBlockAt(rayLocation);

		if (*block != BlockType::AIR && *block != BlockType::WATER)
		{
			if (shouldBreak)
			{
				breakBlockAt(rayLocation);
				return;
			}
			if (!shouldBreak)
			{
				try
				{
					placeBlockAt(rayLocation + getBlockSide(rayLocation), BlockType::SAND);
					return;
				}
				catch(const std::exception& e)
				{
					return;
				}
			}
		}
	}
}

const Texture* World::getTexture() const
{
	return texture;
}

void World::unloadFarChunks(const ChunkLocation_t playerChunkLocation) 
{
	std::vector<std::string> positions;

	for (auto&& [position, chunk] : chunks)
	{
		if(abs(glm::length(chunk->getPosition() - playerChunkLocation)) > RENDER_DISTANCE)
		{
			positions.push_back(position);
		}
	}

	for (auto && position : positions)
	{
		Chunk* current = chunks.at(position);

		chunksMeshQueue.erase(std::remove(chunksMeshQueue.begin(), chunksMeshQueue.end(), 
		current), 
		chunksMeshQueue.end());

		chunksBuildQueue.erase(std::remove(chunksBuildQueue.begin(), chunksBuildQueue.end(), 
		current->getPosition()), 
		chunksBuildQueue.end());
		chunks.erase(position);

		while (true)
		{
			if(!currentlyDrawing && !current->isCurrentlyBuilding() && !current->isCurrentlyDrawing())
			{		
				delete current;
				current = nullptr;
				break;
			}
		}
	}
}

void World::loadNearbyChunks(const ChunkLocation_t& nearby, bool firstTime)
{
	std::vector<ChunkLocation_t> chunksLocations;

	for (int i = nearby.x - RENDER_DISTANCE; i <= nearby.x + RENDER_DISTANCE; i++)
	{
		for (int j = nearby.y - RENDER_DISTANCE; j <= nearby.y + RENDER_DISTANCE; j++)
		{
			std::string positon = vectorToString(ChunkLocation_t(i, j));
			if (chunks.count(positon)) continue;
			chunksLocations.push_back(ChunkLocation_t(i, j));
		}
	}
	
	std::sort(chunksLocations.begin(), chunksLocations.end(), [&](auto& left, auto& right) {
		return glm::length(left - nearby) < glm::length(right - nearby);
	});

	for (auto&& location : chunksLocations)
	{
		loadChunk(location);
	}
}

void World::loadChunk(const ChunkLocation_t& vectorPosition)
{
	std::string positon = vectorToString(vectorPosition);
	if (chunks.count(positon)) return;

	Chunk* chunk = new Chunk(vectorPosition, this, *loader, noise);
	
	chunks.insert({ positon, (chunk) });
	chunksMeshQueue.push_back(chunk);
}

World::Location_t World::getBlockSide(Location_t rayLanding) {
	static const float POSITIVE_SIDE = 0.99f;
	static const float NEGATIVE_SIDE = 0.01;
	static const Location_t allBlockSides[] = GET_NEARBY_BLOCKS(0, 0, 0);

	Location_t decimal(
		rayLanding.x - floor(rayLanding.x),
		rayLanding.y - floor(rayLanding.y),
		rayLanding.z - floor(rayLanding.z)
	);

	int index = -1;

	if (decimal.x >= POSITIVE_SIDE)	index = 3; // +x (1, 0, 0)
	if (decimal.x <= NEGATIVE_SIDE)	index = 2; // -x (-1, 0, 0)
	if (decimal.y >= POSITIVE_SIDE)	index = 4; // +y (0, 1, 0)
	if (decimal.y <= NEGATIVE_SIDE)	index = 5; // -y (0, -1, 0)
	if (decimal.z >= POSITIVE_SIDE)	index = 1; // +z (0, 0, 1)
	if (decimal.z <= NEGATIVE_SIDE)	index = 0; // -z (0, 0, -1)
	
	if(index == -1) throw std::exception();
	return allBlockSides[index];
}
