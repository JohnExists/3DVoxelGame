#include"GameState.h"

GameState::GameState()
{
    isActive = true;

    world = std::make_unique<World>(399393994);    
    player = std::make_unique<Camera>(glm::vec3(0.0f, 75.0f, 0.0f));    
    renderer = std::make_unique<Renderer>(world.get(), player.get());
    // std::thread thr5(cleanUp);
}

void GameState::update(float deltaTime)
{
    std::thread thr1(update1, 1.0f);
    std::thread thr2(update2, 1.0f);
    
    while (isActive)
    {
    }

}

void GameState::update1(float deltaTime)
{
    
}

void GameState::update2(float deltaTime)
{
    
}

void GameState::update3(float deltaTime)
{
    
}

void GameState::render()
{
    
}

void GameState::cleanUp()
{
    
}

//World& GameState::getWorld()
//{
//    // return world.get();
//}

//Camera& GameState::getPlayer()
//{
//    // return player.get();
//}
