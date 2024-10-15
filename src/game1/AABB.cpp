#include "AABB.h"

bool AABB::collidesWith(AABB other)
{
    glm::vec3 box1Min = position;
    glm::vec3 box1Max = position + size;
    glm::vec3 box2Min = other.position;
    glm::vec3 box2Max = other.position + other.size;

    return !(box1Max.x < box2Min.x || box1Min.x > box2Max.x ||
             box1Max.y < box2Min.y || box1Min.y > box2Max.y ||
             box1Max.z < box2Min.z || box1Min.z > box2Max.z);
}

