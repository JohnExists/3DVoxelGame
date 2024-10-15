#include"GameState.h"

//////////////////////////////////
/*		Public Functions		*/
//////////////////////////////////

GameState::GameState()
{
    isActive = true;

    renderer    = std::make_unique<Renderer>();
    world       = std::make_unique<World>(this, 399393994);    
    interface   = std::make_unique<GameInterface>(this);
    player      = std::make_unique<Player>(interface.get(), world.get(), 
        glm::vec3(100.0f, 100.0f, 100.0f));

}

void GameState::update(float deltaTime)
{
    registerInput();
    render();
    player->update(deltaTime);
    world->updateChunksBuilds(&getPlayerCamera(), 0);
    cleanUp();
}

void GameState::render()
{
    renderer->render(world.get(), &getPlayerCamera(), interface.get());
}

void GameState::cleanUp()
{
    glm::vec2 playerLocation = world->getChunkPositionAt(getPlayerCamera().getPosition());
    world->unloadFarChunks(playerLocation);
}

World& GameState::getWorld()
{
   return *world;
}

Player& GameState::getPlayer()
{
   return *player;
}

Camera& GameState::getPlayerCamera()
{
    return player->getCamera();    
}

Renderer& GameState::getRenderer()
{
    return *renderer;    
}

void GameState::disable()
{
    isActive = false;    
}

//////////////////////////////////
/*		Private Functions		*/
//////////////////////////////////


void GameState::registerInput()
{
    using namespace game;
    using enum Movement;
    using enum Action;

    // Keyboard Input
    if(InputHandler::keyIsPressed(SPRINT))              player->sprint();

    if(InputHandler::keyIsPressed(MOVE_FORWARD))        player->move(FORWARD);
    if(InputHandler::keyIsPressed(MOVE_BACKWARD))       player->move(BACKWARD);
    if(InputHandler::keyIsPressed(STRAFE_LEFT))         player->move(LEFT);
    if(InputHandler::keyIsPressed(STRAFE_RIGHT))        player->move(RIGHT);
    if(InputHandler::keyIsPressed(JUMP_OR_FLY_UP))      player->move(UP);
    if(InputHandler::keyIsPressed(CROUCH_OR_FLY_DOWN))  player->move(DOWN);

    for (int key = GLFW_KEY_1; key <= GLFW_KEY_9; key++)
    {
        int slot = key - GLFW_KEY_0;
        if(InputHandler::keyIsPressed(key))             player->selectSlot(slot);
    }
    
    // Mouse Button Input
    if(InputHandler::getMouseStatus() == PUNCH)         player->perform(BREAK_BLOCK);
    if(InputHandler::getMouseStatus() == PLACE)         player->perform(PLACE_BLOCK);

    // Mouse Input
    auto cursorPosition = InputHandler::getCursorPos();
    if(cursorPosition) player->lookAround(cursorPosition.value());

    player->resetSprint();
}
