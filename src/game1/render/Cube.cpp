#include "Cube.h"

Cube::Cube() { }

void Cube::addQuad(Quad quad)
{
    quads.push_back(quad);    
}

void Cube::translate(int x, int y, int z)
{
    for (auto &&quad : quads)
    {
        for (auto &&vertex : quad.vertices)
        {
            vertex.position += glm::vec3(x, y, z);
        }   
    }       
}

void Cube::scale(int x, int y, int z)
{
    applyMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z)));
}

void Cube::rotateX(float angle)
{
    rotate(angle, 1.0f, 0.0f, 0.0f);
}

void Cube::rotateY(float angle)
{
    rotate(angle, 0.0f, 1.0f, 0.0f);    
}

void Cube::rotateZ(float angle)
{
    rotate(angle, 0.0f, 0.0f, 1.0f);    
}

std::vector<Quad>& Cube::getQuads()
{
    return quads;
}

void Cube::rotate(float angle, int x, int y, int z)
{
    applyMatrix(glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(x, y, z)));
}

void Cube::applyMatrix(glm::mat3 matrix)
{
    for (auto &&quad : quads)
    {
        for (auto &&vertex : quad.vertices)
        {
            vertex.position += matrix * vertex.position;
        }   
    }        
}
