#ifndef AABB_H
#define AABB_H

#include <glm/glm.hpp>

struct AABB
{
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 size = glm::vec3(0.0f, 0.0f, 0.0f);
};

#endif // AABB_H
