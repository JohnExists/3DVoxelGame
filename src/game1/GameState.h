#ifndef GAMESTATE_H
#define GAMESTATE_H

#include"world/World.h"
#include"entity/Player.h"
#include"render/Renderer.h"
#include"InputHandler.h"
#include"Settings.h"
#include"ui/GameInterface.h"

#include<utility>

class World;
class Camera;
class Renderer;
class Player;
class GameInterface;

class GameState
{
private:
    // Variables for each component of the game
    std::unique_ptr<World> world;
    std::unique_ptr<Player> player;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<GameInterface> interface;
    bool isActive;
    
public:
    /**
     * Initialize the variables for every
     * component present in the game (world, plater, renderer, interface)
     */
    GameState();
    /**
     * Updates every aspect of the game (input, rendering, player, etc)
     * 
     * @param deltaTime The amount of time between update() calls
     * 
     */
    void update(float deltaTime);    

    /**
     * Handles all the rendering surrounding the entire game
     */
    void render();
    /**
     * Ensures all variables are properly deleted and closes
     * the game safely
     */
    void cleanUp();

    /**
     * @returns Reference to the world associated with the game state
     */
    World& getWorld();
    /**
     * @returns Reference to the main player of the game
     */
    Player& getPlayer();
    /**
     * @returns Reference to the players camera
     */
    Camera& getPlayerCamera();
    /**
     * @returns Reference to the renderer used for drawing objects in the world
     */
    Renderer& getRenderer();

    /**
     * Stops the game from running
     */
    void disable();

private:
    /**
     * Handles all the input surrounding the game and the player
     */
    void registerInput();
};

#endif // GAMESTATE_H
