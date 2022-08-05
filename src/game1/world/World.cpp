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

	preloadChunk(ChunkLocation_t(0, 0), nullptr);
}

Chunk* World::getChunkAt(const ChunkLocation_t& vectorPosition)
{ 
	std::string position = vectorToString(vectorPosition);
	Chunk* chunk = nullptr;
	if (chunks.count(position)) chunk = chunks.at(position).get();

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

glm::vec2 World::getChunkPositionAt(Location_t position)
{
	return glm::vec2(
		floor(position.x / Chunk::MAX_XZ),
		floor(position.z / Chunk::MAX_XZ)
	);
}


void World::draw(Renderer& renderer, Frustum& frustum)
{
	texture->useSlot(renderer.getShader(ShaderType::DEFAULT_SHADER), "texture1", 0);

	int passes = 0;
	int culledPasses = 0;

	for (auto& [position, chunk] : chunks)
	{
		if( frustum.collidesWith(chunk->getAABB()) )
		{
			++culledPasses;
			if(chunk) chunk->draw(renderer, Chunk::PRIMARY_MESH);
		}
	}
	for (auto& [position, chunk] : chunks)
	{
		if( frustum.collidesWith(chunk->getAABB()) )
		{
			if(chunk) chunk->draw(renderer, Chunk::SECONDARY_MESH);
			if(chunk) chunk->draw(renderer, Chunk::TERTIARY_MESH);
		}
	}

	std::cout << "All: " << passes << ": " << "Culled: " << culledPasses << '\n';

}

void World::addToQueue(Chunk* chunk)
{
	chunk->buildBlocks();
}

void World::updateChunksBuilds(Camera* camera, int task) 
{
	Location_t playerLocation = camera->getPosition();
	ChunkLocation_t playerChunk = getChunkPositionAt(playerLocation);
	
	if(task == 0) loadNearbyChunks(playerChunk, camera);
	if(task == 1) unloadFarChunks(playerChunk);

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

		if (*block == BlockType::AIR || *block == BlockType::WATER) continue;

		if (shouldBreak) breakBlockAt(rayLocation);
		if (!shouldBreak) placeBlockAt(rayLocation + getBlockSide(rayLocation), BlockType::SAND);
		break;
	}
}

const Texture* World::getTexture() const
{
	return texture;
}

void World::unloadFarChunks(const ChunkLocation_t playerChunkLocation) 
{
	std::vector<std::string> unloadedChunks;

	for (auto it = chunks.cbegin(); it != chunks.cend(); ++it)	
	{
		Chunk* chunk =  it->second.get();
		
		if(chunk == nullptr) continue;
		if(abs(glm::length(chunk->getPosition() - playerChunkLocation)) >= RENDER_DISTANCE)
		{
			chunk->clearCache();
			unloadedChunks.push_back(it->first);
		}
	}

	for (auto&& unloadedChunk : unloadedChunks)
	{
		// chunks[unloadedChunk] = nullptr;
		// chunks.erase(unloadedChunk);
	}
}        

void World::loadNearbyChunks(const ChunkLocation_t& nearby, Camera* camera)
{
	std::vector<ChunkLocation_t> chunksLocations;

	for (int x = nearby.x - RENDER_DISTANCE; x <= nearby.x + RENDER_DISTANCE; x++)
	{
		for (int z = nearby.y - RENDER_DISTANCE; z <= nearby.y + RENDER_DISTANCE; z++)
		{
			ChunkLocation_t location(x, z);
			std::string positon = vectorToString(ChunkLocation_t(x, z));
			if (abs(glm::length(location - nearby)) > RENDER_DISTANCE) continue;
			if (chunks.count(positon)) continue;
		
			chunksLocations.push_back(ChunkLocation_t(x, z));
		}
	}

	std::sort(chunksLocations.begin(), chunksLocations.end(), 
		[&] (const auto& left, const auto& right) {
			return glm::length(left - nearby) < glm::length(right - nearby);
		});

	for (auto&& location : chunksLocations)
	{
		preloadChunk(location, camera);
	}
}

void World::preloadChunk(const ChunkLocation_t& vectorPosition, Camera* camera)
{
	std::string positon = vectorToString(vectorPosition);
	if(camera != nullptr)
	{
		ChunkLocation_t cameraLocation = getChunkPositionAt(camera->getPosition());
		if (abs(glm::length(cameraLocation - vectorPosition)) > RENDER_DISTANCE) 
		{
			return;	
		}
	}
 
	chunks.insert({ 
		positon, 
		(std::make_unique<Chunk>(vectorPosition, this, *loader, noise)) 
	});

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
	
	if(index == -1) return World::Location_t(0, 0, 0);
	return allBlockSides[index];
}
