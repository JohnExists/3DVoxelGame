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
    // All the mesh vertices for the user interface
    std::unique_ptr<Mesh> primary;
    std::unique_ptr<Mesh> secondary;
    // The textures for the user interface
    std::unique_ptr<Texture> uiTexture;
    std::unique_ptr<Texture> blockTexture;
    // All the elements of the UI
    std::vector<InterfaceElement> elements;

    const float WIDTH = 206;
    const float HEIGHT = 38;

public:
    /**
     * @brief Initializes all objects for user interface (such as hotbar, crosshair, etc)
     * 
     * @param gameState Pointer to the current GameState object present
     */
    GameInterface(GameState* gameState);
    /**
     * @brief Sets up the mesh for the interface to be displayed
     */
    void setupMesh();

    /**
     * @brief Loads a UI element
     * 
     * @param path The .json file path relative to /res
     * @param state The state of that current UI element
     */
    void addElement(std::string path, float state);
    /**
     * @brief Loads a UI element
     * 
     * @param path The .json file path relative to /res
     */
    void addElement(std::string path);
    /**
     * @brief Replace a current element with a brand new one
     * 
     * @param path The .json file path relative to /res
     * @param state The state of that current UI element
     * @param atlas The texture atlas for the new element
     */
    void addAlternateElement(std::string path, float state, Atlas atlas);
    /**
     * @brief Removes a certain element by name
     * 
     * @param name The name of the element that will be removed
     */
    void removeElement(std::string name);

    /**
     * @brief Draws the current user itnerface
     * 
     * @param renderer Reference to the renderer that will render the object
     */
    void draw(Renderer& renderer);

    /**
     * @brief Generates blocks for the user hotbar
     * 
     */    
    void loadBlock();

private:
    InterfaceElement createElement(nlohmann::json data, float state, Atlas atlas);
};



#endif // GAMEINTERFACE_H
