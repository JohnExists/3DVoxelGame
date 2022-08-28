#include "GameInterface.h"

GameInterface::GameInterface(GameState* gameState)
{
    texture = std::make_unique<Texture>("../res/gui.png");
    blockTexture = std::make_unique<Texture>("../res/atlas.png");

    texture->setScalingFilter(GL_NEAREST);
    blockTexture->setScalingFilter(GL_NEAREST);

    mesh = std::make_unique<Mesh>(texture.get(), &gameState->getRenderer().getShaderAt("user_interface"));
    // mesh->scale(0.075, 0.075, 0.075);
    mesh->scale(1.0, 1.7, 0);

    blockMesh = std::make_unique<Mesh>(blockTexture.get(), &gameState->getRenderer()
        .getShaderAt("block_hotbar_select"));

    glm::vec3 positon = glm::vec3(0.0f, 0.0f, -0.1f);
    camera = std::make_unique<Camera>(positon);

    addElement("crosshair.json");
    addElement("hotbar_select.json", 4);
    addElement("hotbar.json");

    setupMesh();

    // loadBlock();
}

void GameInterface::setupMesh()
{
    std::sort(elements.begin(), elements.end(),
        [&] (const auto& left, const auto& right) {
            return left.layoutOrder > right.layoutOrder;
    });

    mesh->clear();
    blockMesh->clear();

    for (auto &&element : elements)
    {
        Mesh* meshUsed;
        if(element.meshType == "PRIMARY") meshUsed = mesh.get();
        if(element.meshType == "SECONDARY") meshUsed = blockMesh.get();
        meshUsed->addRect(element.position, element.size, element.atlas);
    }
    mesh->loadVertexObject();
    blockMesh->loadVertexObject();

}

void GameInterface::generateElementMesh(InterfaceElement element)
{
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
    mesh->draw();
    glDepthMask(GL_TRUE);
    blockMesh->draw();
}

void GameInterface::loadBlock()
{
    nlohmann::json data;
    std::ifstream file("../res/tweaker.json");
    file >> data;

    Block block = Block(BlockType::GRASS);
    auto tex = BlockBuilder::genTexCoords(block);
    
    blockMesh->addRect(glm::vec2(data["x1"], data["y1"]), glm::vec2(0.01f, 0.01f), tex[0]);
    blockMesh->loadVertexObject();
}

Camera& GameInterface::getCamera()
{
    return *camera;    
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
