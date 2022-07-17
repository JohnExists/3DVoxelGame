#ifndef GAMESTATE_H
#define GAMESTATE_H

#include"world/World.h"
#include"Camera.h"
#include"render/Renderer.h"

#include<utility>

class GameState
{
private:
    std::unique_ptr<World> world;
    std::unique_ptr<Camera> player;
    std::unique_ptr<Renderer> renderer;

    bool isActive;
    
public:
    GameState();
    void update1(float deltaTime);    
    void update2(float deltaTime);    

    void render();
    void cleanUp();

    World& getWorld();
    Camera& getPlayer();

private:
};


#endif // GAMESTATE_H
