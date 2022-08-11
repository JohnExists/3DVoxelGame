#ifndef GAMESTATE_H
#define GAMESTATE_H

#include"world/World.h"
#include"entity/Player.h"
#include"render/Renderer.h"
#include"InputHandler.h"
#include"Settings.h"

#include<utility>

class GameState
{
private:
    std::unique_ptr<World> world;
    std::unique_ptr<Player> player;
    std::unique_ptr<Renderer> renderer;

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

    void disable();

private:
    void registerInput();
};

#endif // GAMESTATE_H
