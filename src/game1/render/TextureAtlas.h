#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H

#include<array>
#include<string>
#include<fstream>
#include<iostream>

#include<glm/glm.hpp>

#include<JSONParser.h>


struct Atlas
{
    glm::vec2 location;
    glm::vec2 size;
};

namespace tex
{
    using AtlasCollection_t = std::array<Atlas, 6>;
    using LoaderFormat = std::array<std::string, 6>;

    AtlasCollection_t load(std::string path);
    Atlas JSONLoader(std::string face, const nlohmann::json& data);
    AtlasCollection_t getBlockTex(const nlohmann::json& data, LoaderFormat format);


}


#endif // TEXTUREATLAS_H
