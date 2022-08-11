#include "Player.h"


Player::Player(World* world, glm::vec3 position) : currentWorld(world)
{
    camera = std::make_unique<Camera>(position);
}

Camera& Player::getCamera()
{
    return *camera;
}

void Player::update(float deltaTime)
{
    camera->move(velocity * deltaTime);
    resetVelocity();
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
    case Movement::UP:          return Velocity_t(0.0f, 25.0f, 0.0f);
    case Movement::DOWN:        return Velocity_t(0.0f, -25.0f, 0.0f);
    }
    return Velocity_t();
}


void Player::castRay(Action action)
{
    //if(action != Action::BREAK_BLOCK) return;
    //if(action != Action::PLACE_BLOCK) return;
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
        case Action::BREAK_BLOCK: currentWorld->breakBlockAt(rayLocation);
            break;
        case Action::PLACE_BLOCK: currentWorld->placeBlockAt(rayLocation + getBlockSide(rayLocation), BlockType::SAND);
            break;
        }
        return;
	}
}


game::Location_t Player::getBlockSide(game::Location_t rayLanding) {
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
