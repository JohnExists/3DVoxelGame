#include "Player.h"


Player::Player(GameInterface* interface, World* world, glm::vec3 position) 
    : currentWorld(world), interface(interface)
{
    camera = std::make_unique<Camera>(position);
	world->preloadChunk(game::ChunkLocation_t(0, 0), nullptr);

    for (auto &&slot : hotbar)
    {
        slot = BlockType::AIR;
    }
    

    hotbar[0] = BlockType::GRASS;
    hotbar[1] = BlockType::GRASS_BLADES;
    hotbar[2] = BlockType::LEAVES;
}

Camera& Player::getCamera()
{
    return *camera;
}

void Player::update(float deltaTime)
{
    updateJump(deltaTime);
    updateFall(deltaTime);
    updateMovement(deltaTime);
    resetVelocity();
    updateInventory();
}

void Player::move(Movement playerMovement)
{
    velocity += getMovementDirection(playerMovement);           
}

void Player::sprint()
{
    currentlySprinting = true;
    movementSpeed += 25;    
}

void Player::resetSprint()
{
    if(currentlySprinting) movementSpeed -= 25;
    currentlySprinting = false;    
}

void Player::updateMovement(float deltaTime)
{
    glm::vec3 travel = velocity * deltaTime;
    if(velocity != glm::vec3(0.0f, 0.0f, 0.0f))
    {
        AABB xHitbox = generateHitbox(getPosition() + glm::vec3(travel.x, 0.0f, 0.0f));
        if(!currentWorld->collidesWithSurface(xHitbox))
        {
            camera->move(glm::vec3(travel.x, 0.0f, 0.0f));
        }

        AABB yHitbox = generateHitbox(getPosition() + glm::vec3(0.0f, travel.y, 0.0f));
        if(!currentWorld->collidesWithSurface(yHitbox))
        {
            camera->move(glm::vec3(0.0f, travel.y, 0.0f));
            landed = false;
        }
        else
        {
            landed = true;
        }

        AABB zHitbox = generateHitbox(getPosition() + glm::vec3(0.0f, 0.0f, travel.z));
        if(!currentWorld->collidesWithSurface(zHitbox))
        {
            camera->move(glm::vec3(0.0f, 0.0f, travel.z));
        }
    }
}

void Player::updateFall(float deltaTime)
{
    static float timeFalling = 0.0f;
    if(landed) 
    {
        timeFalling = 0.0f;
        return;
    }
    timeFalling += deltaTime;
    float distanceToFall = -(392.0f/35.0f) * timeFalling;
    velocity += Velocity_t(0.0, distanceToFall, 0.0f);
}

void Player::updateJump(float deltaTime)
{
    static const float JUMP_SPEED_PER_SECOND = 15.0f;
    static float distanceJumped = 0.0f;

    if(!jumping) return;
    if(distanceJumped >= 2.5f) 
    {
        distanceJumped = 0.0f; 
        jumping = false;
        return;
    }
    distanceJumped += JUMP_SPEED_PER_SECOND * deltaTime;
    velocity += Velocity_t(0.0f, JUMP_SPEED_PER_SECOND, 0.0f);
}

void Player::perform(Action action)
{
    
    switch (action)
    {
    case Action::BREAK_BLOCK: castRay(Action::BREAK_BLOCK);
        return;
    case Action::PLACE_BLOCK: castRay(Action::PLACE_BLOCK);
        return;
    
    }
}

void Player::resetVelocity()
{
    velocity = Velocity_t(0.0f, 0.0f, 0.0f);
}

void Player::lookAround(game::CursorLocation_t newCursorLocation)
{
    static bool firstTime = true;
    static game::CursorLocation_t oldCursorLocation;
    
    if(firstTime)
    {
        oldCursorLocation = newCursorLocation;
        firstTime = false;
    }
    
    game::CursorLocation_t distanceMoved(
        newCursorLocation.x - oldCursorLocation.x,
        oldCursorLocation.y - newCursorLocation.y
    );

    camera->lookAround(distanceMoved);

    oldCursorLocation = newCursorLocation;

}

void Player::selectSlot(int slot)
{
    selectedSlot = slot - 1;
    interface->removeElement("hotbar_select");
    interface->addElement("hotbar_select.json", slot);
    interface->setupMesh();
}

void Player::give(BlockType block)
{
    for (size_t i = 0; i < hotbar.size(); i++)
    {
        if(hotbar[i] != BlockType::AIR) continue;
        hotbar[i] = block;
        return;
    }
}

glm::vec3 Player::getPosition()
{
    return camera->getPosition();    
}

AABB Player::generateHitbox(glm::vec3 position)
{
    AABB currentHitbox;
    const glm::vec3 hitBoxSize = glm::vec3(0.6, 1.8, 0.6);

    currentHitbox.position = position - (hitBoxSize / 2.0f) - glm::vec3(0.0f, 0.6f, 0.0f);
    currentHitbox.size = hitBoxSize;

    return currentHitbox;
}


float Player::getMovementSpeed()
{
    int extraSpeed = 0;
    if(currentlySprinting) extraSpeed += 25;

    return movementSpeed + extraSpeed;
}


Player::Velocity_t Player::getMovementDirection(Movement movementDirection)
{
    glm::vec3 front = camera->getFrontVector();
    glm::vec3 right = camera->getRightVector();

    float speed = movementSpeed + getMovementSpeed();

    switch (movementDirection)
    {
    case Movement::FORWARD:     return front * speed;
    case Movement::BACKWARD:    return -front * speed;
    case Movement::RIGHT:       return right * speed;
    case Movement::LEFT:        return -right * speed;
    case Movement::UP:          if(landed) jumping = true; return Velocity_t(0.0f, 0.0f, 0.0f);
    case Movement::DOWN:        return Velocity_t(0.0f, -25.0f, 0.0f);
    }
    return Velocity_t();
}


void Player::castRay(Action action)
{
	static const float PLAYER_REACH = 6.0f;

	game::Location_t castFrom = camera->getPosition();
	glm::vec3 castTo = camera->getDirectionVector();

	for (float rayLength = 0; rayLength <= PLAYER_REACH; rayLength += 0.01)
	{
		game::Location_t rayLocation = castFrom + (castTo * rayLength);
		Block* block = currentWorld->getBlockAt(rayLocation);

		if (*block == BlockType::AIR || *block == BlockType::WATER) continue;

        switch (action)
        {
        case Action::BREAK_BLOCK: 
            {
                BlockType block = currentWorld->breakBlockAt(rayLocation);
                give(block);
                break;
            }
        case Action::PLACE_BLOCK: 
            {
                if(hotbar[selectedSlot] == BlockType::AIR) return;
                game::Location_t blockLocation = rayLocation + getBlockSide(rayLocation);
                if(abs( glm::length(blockLocation - getPosition() )) > 1.25f)
                {
                    currentWorld->placeBlockAt(blockLocation, hotbar[selectedSlot]);
                }
                break;
            }
        }
        return;
	}
}


game::Location_t Player::getBlockSide(game::Location_t rayLanding) 
{
	static const float POSITIVE_SIDE = 0.99f;
	static const float NEGATIVE_SIDE = 0.01;
	static const game::Location_t allBlockSides[] = GET_NEARBY_BLOCKS(0, 0, 0);

	game::Location_t decimal(
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
	
	if(index == -1) return game::Location_t(0, 0, 0);
	return allBlockSides[index];
}

void Player::updateInventory()
{
    interface->removeElement("hotbar_item");
    
    int counter = 0;
    for (auto&& slot : hotbar)
    {
        ++counter;
        if(slot == BlockType::AIR) continue;    
        interface->addAlternateElement("hotbar_item.json", counter, 
            BlockBuilder::genTexCoords(slot)[0]);
    }
    interface->setupMesh();      
}