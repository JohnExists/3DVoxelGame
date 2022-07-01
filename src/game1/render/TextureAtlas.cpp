#include "TextureAtlas.h"


tex::AtlasCollection_t tex::load(std::string path) 
{
    nlohmann::json data;
    std::ifstream file("../res/blocks/" + path);

    file >> data;

    int uniqueFaceCount = data["unique-face-count"];
    tex::LoaderFormat format;

    assert((uniqueFaceCount == 1 || uniqueFaceCount == 3) && "Invalid Face Count");
    if(uniqueFaceCount == 1) format = {"sides", "sides", "sides", "sides", "sides", "sides"};
    if(uniqueFaceCount == 3) format = {"sides", "sides", "sides", "sides", "bottom", "top"};

    return getBlockTex(data, format);
}

Atlas tex::JSONLoader(std::string face, const nlohmann::json& data) 
{
    Atlas atlas;

    auto currentData = data.at(face);
    auto location = currentData.at("location");
    auto size = currentData.at("size");

    atlas.location.x = location.at(0);
    atlas.location.y = location.at(1);

    atlas.size.x = size.at(0);
    atlas.size.y = size.at(1);

    return atlas;
}

tex::AtlasCollection_t tex::getBlockTex(const nlohmann::json& data, tex::LoaderFormat format)
{
    tex::AtlasCollection_t collection;

    for (int i = 0; i < 6; i++)
    {
        collection[i] = JSONLoader(format[i], data);
    }

    return collection;
}

