#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include<utility>
#include<fstream>
#include<vector>
#include<algorithm>

#include<JSONParser.h>

#include"../render/Texture.h"
#include"../render/TextureAtlas.h"
#include"../render/Mesh.h"
#include"../Settings.h"
#include"../render/CubeBuilder.h"
#include"../Camera.h"

class Renderer;
class GameState;
class Mesh;
class Texture;
class Camera;

struct InterfaceElement
{
    std::string name;
    Atlas atlas;
    glm::vec2 position;
    glm::vec2 size;
    int layoutOrder;
    std::string meshType;
};


class GameInterface
{
private:
    std::unique_ptr<Mesh> mesh;
    std::unique_ptr<Texture> texture;
    
    std::unique_ptr<Mesh> blockMesh;
    std::unique_ptr<Texture> blockTexture;

    std::unique_ptr<Camera> camera;

    std::vector<InterfaceElement> elements;

    const float WIDTH = 206;
    const float HEIGHT = 38;

public:
    GameInterface(GameState* gameState);
    void setupMesh();
    void generateElementMesh(InterfaceElement element);

    void addAlternateElement(std::string path, float state, Atlas atlas);
    void addElement(std::string path, float state);
    void addElement(std::string path);
    void removeElement(std::string name);

    void draw(Renderer& renderer);

    void loadBlock();

    Camera& getCamera();

private:
    InterfaceElement createElement(nlohmann::json data, float state, Atlas atlas);
};



#endif // GAMEINTERFACE_H
