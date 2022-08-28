#ifndef PLAYER_H
#define PLAYER_H

#include<utility>
#include<array>

#include "../world/World.h"
#include "../Camera.h"
#include "../Settings.h"
#include "../ui/GameInterface.h"

class World;
class Camera;
class GameInterface;

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
    GameInterface* interface;

    std::unique_ptr<Camera> camera;
    bool currentlySprinting = false;
    float movementSpeed = 25;
    Velocity_t velocity;

    std::array<BlockType, 9> hotbar;
    int selectedSlot;

public:

    Player(GameInterface* interface, World* world, glm::vec3 position);

    Camera& getCamera();

    void update(float deltaTime);

    void move(Movement playerMovement);
    void sprint();
    void resetSprint();

    void perform(Action action);
    void resetVelocity();
    void lookAround(game::CursorLocation_t newCursorLocation);
    void selectSlot(int slot);
private:
    void castRay(Action action);
    game::Location_t getBlockSide(game::Location_t rayLanding);

    Velocity_t getMovementDirection(Movement playerMovement);
    float getMovementSpeed();
    void updateInventory();
};

#endif // PLAYER_H
