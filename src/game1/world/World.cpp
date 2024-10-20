#include "World.h"

std::string World::vectorToString(const glm::vec2& vec)
{
	return std::to_string(vec.x) + ':' +
		std::to_string(vec.y);
}

World::World(GameState* gameState, int seed) : gameState(gameState)
{
	noise = new FastNoiseLite(seed);
	loader = new BiomeLoader(this, *noise);
	noise->setNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);
	noise->setFractalOctaves(16);

	this->texture = new Texture("../res/atlas.png");
	texture->setWrapAround(GL_CLAMP_TO_BORDER);
	texture->setScalingFilter(GL_NEAREST);
}

Chunk* World::getChunkAt(const game::ChunkLocation_t& vectorPosition)
{ 
	std::string position = vectorToString(vectorPosition);
	Chunk* chunk = nullptr;
	if (chunks.count(position)) chunk = chunks.at(position).get();

	return chunk;
}

Chunk* World::getChunkAtWorld(const game::Location_t& position)
{
	game::ChunkLocation_t chunkPosition = glm::vec2(
		floor(position.x / Chunk::MAX_XZ),
		floor(position.z / Chunk::MAX_XZ)
	);

	return getChunkAt(chunkPosition);
}

Block* World::getBlockAt(game::Location_t position)
{
	Chunk* chunk = getChunkAtWorld(position);
	if (chunk == nullptr) return nullptr;
	return &chunk->getBlockAt(position);
}

glm::vec2 World::getChunkPositionAt(game::Location_t position)
{
	return glm::vec2(
		floor(position.x / Chunk::MAX_XZ),
		floor(position.z / Chunk::MAX_XZ)
	);
}


void World::draw(Renderer& renderer, Frustum& frustum)
{
	for (auto& [position, chunk] : chunks)
	{
		if( frustum.collidesWith(chunk->getAABB()) )
		{
			if(chunk) chunk->draw(renderer, Chunk::PRIMARY_MESH);
		}
	}
	for (auto& [position, chunk] : chunks)
	{
		if( frustum.collidesWith(chunk->getAABB()) )
		{
			if(chunk) chunk->draw(renderer, Chunk::SECONDARY_MESH);
		}
	}
}

void World::addToQueue(Chunk* chunk)
{
	chunk->buildBlocks();
}

void World::updateChunksBuilds(Camera* camera, int task) 
{
	game::Location_t playerLocation = camera->getPosition();
	game::ChunkLocation_t playerChunk = getChunkPositionAt(playerLocation);
	
	if(task == 0) loadNearbyChunks(playerChunk, camera);
	if(task == 1) unloadFarChunks(playerChunk);

}

void World::clearGarabage()
{
	while (!garbageChunks.empty())
	{
		chunks[garbageChunks.front()]->clearCache();
		chunks.erase(garbageChunks.front());
		garbageChunks.pop();
	}
}

BlockType World::breakBlockAt(game::Location_t position)
{
	Chunk* chunk = getChunkAtWorld(position);
	BlockType block = chunk->getBlockAt(position).getType();
	chunk->setBlockAt(position, BlockType::AIR, true);

	return block;
}

void World::placeBlockAt(game::Location_t position, BlockType type)
{
	Chunk* chunk = getChunkAtWorld(position);
	chunk->setBlockAt(position, type, false);
}

const Texture* World::getTexture() const
{
	return texture;
}

GameState& World::getGameState()
{
	return *gameState;	
}

void World::unloadFarChunks(const game::ChunkLocation_t playerChunkLocation) 
{
	std::vector<std::string> removeableChunks;
	for (auto it = chunks.cbegin(); it != chunks.cend(); ++it)	
	{
		Chunk* chunk =  it->second.get();

		if(chunk == nullptr) continue;
		if(abs(glm::length(chunk->getPosition() - playerChunkLocation)) >= RENDER_DISTANCE)
		{
			chunk->clearCache();
			removeableChunks.push_back(it->first);
		}
	}

	for (auto &&removeable : removeableChunks)
	{
		std::unique_ptr<Chunk>& chunk = chunks.at(removeable);
		chunks.erase(removeable);
	}
	
}        

void World::loadNearbyChunks(const game::ChunkLocation_t& nearby, Camera* camera)
{
	std::vector<game::ChunkLocation_t> chunksLocations;

	for (int x = nearby.x - RENDER_DISTANCE; x <= nearby.x + RENDER_DISTANCE; x++)
	{
		for (int z = nearby.y - RENDER_DISTANCE; z <= nearby.y + RENDER_DISTANCE; z++)
		{
			game::ChunkLocation_t location(x, z);
			std::string positon = vectorToString(game::ChunkLocation_t(x, z));
			if (abs(glm::length(location - nearby)) > RENDER_DISTANCE) continue;
			if (chunks.count(positon)) continue;
		
			chunksLocations.push_back(game::ChunkLocation_t(x, z));
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

void World::preloadChunk(const game::ChunkLocation_t& vectorPosition, Camera* camera)
{
	std::string positon = vectorToString(vectorPosition);
	if(camera != nullptr)
	{
		game::ChunkLocation_t cameraLocation = getChunkPositionAt(camera->getPosition());
		if (abs(glm::length(cameraLocation - vectorPosition)) > RENDER_DISTANCE) return;	
	}
 
	chunks.insert({ 
		positon, 
		(std::make_unique<Chunk>(vectorPosition, this, *loader)) 
	});
}

bool World::collidesWithSurface(AABB object)
{
	glm::vec3 objectMin = object.position;
	glm::vec3 objectMax = object.position + object.size;

	for (int x = floor(objectMin.x); x <= ceil(objectMax.x); x++)
	{
		for (int z = floor(objectMin.z); z <= ceil(objectMax.z); z++)
		{
			Chunk* chunk = getChunkAtWorld(game::Location_t(x, 0.0f, z));
			if(!chunk) continue;

			for (int y = floor(objectMin.y); y <= ceil(objectMax.y); y++)
			{
				game::Location_t location = game::Location_t(x, y, z);
				Block* block = &chunk->getBlockAt(location);
				if(!block) continue;
				if(!block->isSolid()) continue;
				AABB currentAABB = AABB(location, glm::vec3(1.0f, 1.0f, 1.0f));
				if(currentAABB.collidesWith(object)) return true;
			}
		}
	}

	return false;
}
