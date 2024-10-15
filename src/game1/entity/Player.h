#ifndef PLAYER_H
#define PLAYER_H

#include<utility>
#include<array>

#include "../world/World.h"
#include "../Camera.h"
#include "../Settings.h"
#include "../ui/GameInterface.h"
#include "../AABB.h"

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
    float movementSpeed = 5;
    Velocity_t velocity;

    std::array<BlockType, 9> hotbar;
    int selectedSlot = 0;
    AABB hitBox;
    bool landed = false;
    bool jumping = false;
public:
    /**
     * Constructs a new Player object.
     *
     * @param interface A pointer to the game interface.
     * @param world A pointer to the game world.
     * @param position The initial position of the player in the world as a glm::vec3.
     */
    Player(GameInterface* interface, World* world, glm::vec3 position);

    /**
     * @return A reference to the Camera object associated with the player.
     */
    Camera& getCamera();

    /**
     * Updates the player's state.
     *
     * @param deltaTime The time elapsed since the last update, in seconds.
     */
    void update(float deltaTime);

    /**
     * Moves the player based on the specified movement type.
     *
     * @param playerMovement The type of movement to perform.
     */
    void move(Movement playerMovement);
    /**
     * Enables sprinting for the player.
     */
    void sprint();
    /**
     * Resets the player's sprint state.
     */
    void resetSprint();

    /**
     * Updates the player's movement based on the time elapsed.
     *
     * @param deltaTime The time elapsed since the last update, in seconds.
     */
    void updateMovement(float deltaTime);
    /**
     * Updates the player's fall state based on the time elapsed.
     *
     * @param deltaTime The time elapsed since the last update, in seconds.
     */
    void updateFall(float deltaTime);
    /**
     * Updates the player's jump state based on the time elapsed.
     *
     * @param deltaTime The time elapsed since the last update, in seconds.
     */
    void updateJump(float deltaTime);

    /**
     * Performs the specified action.
     *
     * @param action The action to perform.
     */
    void perform(Action action);
    /**
     * Resets the player's velocity to zero.
     */
    void resetVelocity();
    /**
     * Updates the player's view direction based on the new cursor location.
     *
     * @param newCursorLocation The new location of the cursor.
     */
    void lookAround(game::CursorLocation_t newCursorLocation);
    /**
     * Selects the specified slot in the player's inventory.
     *
     * @param slot The inventory slot to select.
     */
    void selectSlot(int slot);

    /**
     * Gives the player a block of the specified type.
     *
     * @param block The type of block to give to the player.
     */
    void give(BlockType block);

    glm::vec3 getPosition();

private:
    AABB generateHitbox(glm::vec3 positon);
    void castRay(Action action);
    game::Location_t getBlockSide(game::Location_t rayLanding);

    Velocity_t getMovementDirection(Movement playerMovement);
    float getMovementSpeed();
    void updateInventory();
};

#endif // PLAYER_H
