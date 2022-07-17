#include"GameState.h"

GameState::GameState()
{
    isActive = true;

    world = std::make_unique<World>(399393994);    
    player = std::make_unique<Camera>(glm::vec3(0.0f, 75.0f, 0.0f));    
    renderer = std::make_unique<Renderer>(world.get(), player.get());
    // std::thread thr5(cleanUp);
}

void GameState::update1(float deltaTime)
{
    
}

void GameState::update2(float deltaTime)
{
    
}

void GameState::render()
{
    renderer->render();    
}

void GameState::cleanUp()
{
    world->updateChunksBuilds(player.get(), 1);
}

World& GameState::getWorld()
{
   return *world;
}

Camera& GameState::getPlayer()
{
   return *player;
}
