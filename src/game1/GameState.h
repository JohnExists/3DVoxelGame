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
    std::unique_ptr<World> world;
    std::unique_ptr<Player> player;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<GameInterface> interface;
    bool isActive;
    
public:
    GameState();
    void update(float deltaTime);    
    void update2();    

    void render();
    void cleanUp();

    World& getWorld();
    Player& getPlayer();
    Camera& getPlayerCamera();
    Renderer& getRenderer();

    void disable();

private:
    void registerInput();
};

#endif // GAMESTATE_H
