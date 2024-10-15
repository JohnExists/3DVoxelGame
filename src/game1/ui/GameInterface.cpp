#include "GameInterface.h"

GameInterface::GameInterface(GameState* gameState)
{
    uiTexture = std::make_unique<Texture>("../res/gui.png");
    blockTexture = std::make_unique<Texture>("../res/atlas.png");

    uiTexture->setScalingFilter(GL_NEAREST);
    blockTexture->setScalingFilter(GL_NEAREST);

    primary = std::make_unique<Mesh>(uiTexture.get(), &gameState->getRenderer().getShaderAt("user_interface"));
    primary->scale(1.0, 1.7, 1.0);

    secondary = std::make_unique<Mesh>(blockTexture.get(), &gameState->getRenderer()
        .getShaderAt("user_interface"));
    secondary->scale(1.0, 1.7, 1.0);

    addElement("crosshair.json");
    addElement("hotbar_select.json", 4);
    addElement("hotbar.json");

    setupMesh();
}

void GameInterface::setupMesh()
{
    std::sort(elements.begin(), elements.end(),
        [&] (const auto& left, const auto& right) {
            return left.layoutOrder > right.layoutOrder;
    });

    primary->clear();
    secondary->clear();

    for (auto &&element : elements)
    {
        Mesh* meshUsed;
        if(element.meshType == "PRIMARY") meshUsed = primary.get();
        if(element.meshType == "SECONDARY") meshUsed = secondary.get();
        meshUsed->addRect(element.position, element.size, element.atlas);
    }
    primary->loadVertexObject();
    secondary->loadVertexObject();

}

void GameInterface::addAlternateElement(std::string path, float state, Atlas atlas)
{
    nlohmann::json data;
    std::ifstream file("../res/ui/" + path);
    file >> data;

    --state;
    
    elements.push_back(createElement(data, state, atlas));
}

void GameInterface::addElement(std::string path, float state)
{
    const static glm::vec2 imageSize(WIDTH, HEIGHT);
    nlohmann::json data;
    std::ifstream file("../res/ui/" + path);
    file >> data;

    Atlas atlas;
    --state;
    
    atlas = tex::JSONLoader("atlas", data);
    atlas.location /= imageSize;
    atlas.size /= imageSize;

    elements.push_back(createElement(data, state, atlas));
}

void GameInterface::addElement(std::string path)
{
    addElement(path, 0);
}

void GameInterface::removeElement(std::string name)
{
    elements.erase(std::remove_if(elements.begin(), elements.end(), 
        [&](const auto& element) {
            return element.name == name;
        }),
    elements.end());
}

void GameInterface::draw(Renderer& renderer)
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glDepthMask(GL_FALSE);
    primary->draw();
    glDepthMask(GL_TRUE);
    secondary->draw();
}

void GameInterface::loadBlock()
{
    nlohmann::json data;
    std::ifstream file("../res/tweaker.json");
    file >> data;

    Block block = Block(BlockType::GRASS);
    auto tex = BlockBuilder::genTexCoords(block);
    
    secondary->addRect(glm::vec2(data["x1"], data["y1"]), glm::vec2(0.01f, 0.01f), tex[0]);
    secondary->loadVertexObject();
}

InterfaceElement GameInterface::createElement(nlohmann::json data, float state, Atlas atlas)
{
    InterfaceElement element;

    element.atlas = atlas;
    glm::vec2 gap = jsonDataToVec2(data["gap"]);
    element.position = jsonDataToVec2(data["location"]) + (gap * state);
    element.size = jsonDataToVec2(data["size"]);
    element.name = data["name"];
    element.layoutOrder = data["layout-order"];
    element.meshType= data["mesh-type"];

    return element;    
}
