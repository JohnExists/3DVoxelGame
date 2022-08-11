#ifndef PLAYER_H
#define PLAYER_H

#include<utility>

#include"../world/World.h"

#include "../Camera.h"
#include "../Settings.h"

enum class Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

enum class Action
{
    BREAK_BLOCK,
    PLACE_BLOCK
};

class Player
{
private:
    using Velocity_t = glm::vec3;

    World* currentWorld;

    std::unique_ptr<Camera> camera;
    bool currentlySprinting = false;
    float movementSpeed = 25;
    Velocity_t velocity;

    
public:

    Player(World* world, glm::vec3 position);

    Camera& getCamera();

    void update(float deltaTime);

    void move(Movement playerMovement);
    void sprint();
    void resetSprint();

    void perform(Action action);

    void resetVelocity();

    void lookAround(game::CursorLocation_t newCursorLocation);

private:
    void castRay(Action action);
    game::Location_t getBlockSide(game::Location_t rayLanding);

    Velocity_t getMovementDirection(Movement playerMovement);
    float getMovementSpeed();
};

#endif // PLAYER_H
