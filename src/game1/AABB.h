#ifndef AABB_H
#define AABB_H

#include <glm/glm.hpp>

struct AABB
{
    // The location of this collision box
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    // The size of this collision box
    glm::vec3 size = glm::vec3(0.0f, 0.0f, 0.0f);

    /**
     * Checks if it collides with the other collision box
     * 
     * @param other The other collision box to check against
     * @returns Whether there is a collision
     */
    bool collidesWith(AABB other);
};

#endif // AABB_H
