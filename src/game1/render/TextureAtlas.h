#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H

#include<array>
#include<string>
#include<fstream>
#include<iostream>

#include<glm/glm.hpp>

#include<JSONParser.h>

#define jsonDataToVec2(data) glm::vec2(data[0], data[1])

struct Atlas
{
    glm::vec2 location;
    glm::vec2 size;
};

namespace tex
{
    // Useful atlas type aliases
    using AtlasCollection_t = std::array<Atlas, 6>;
    using LoaderFormat = std::array<std::string, 6>;

    /**
     * @param path Path to the texture atlas data relative to "../res/blocks/"
     * 
     * @returns Loads a texture atlas data from a specific .json file
     */
    AtlasCollection_t load(std::string path);
    /**
     * @param face The face that will be loaded ("sides", "top", etc)
     * @param data The json data that is used to generate face data
     * 
     * @returns Atlas with all the JSON data for that specific dace
     */
    Atlas JSONLoader(std::string face, const nlohmann::json& data);

    /**
     * @brief Get the Block Tex object
     * 
     * @param data The data that will generate the tex values
     * @param format The format of that data as a list of strings
     * @return The list of atlases that are generated from that data
     */
    AtlasCollection_t getBlockTex(const nlohmann::json& data, LoaderFormat format);


}


#endif // TEXTUREATLAS_H
