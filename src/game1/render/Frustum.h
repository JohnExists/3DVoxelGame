#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <glm/glm.hpp>

#include "../AABB.h"

struct Plane
{
    glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
    float distance = 0.f;

    Plane() { }

    Plane(const glm::vec3 p1, const glm::vec3 norm)
        : normal(glm::normalize(norm)), distance(glm::dot(normal, p1)) { }

    float getSignedDistanceTo(glm::vec3 point);
private:

};

class Frustum
{
public:
    Plane topFace;
    Plane bottomFace;

    Plane rightFace;
    Plane leftFace;

    Plane farFace;
    Plane nearFace;

    bool collidesWith(const AABB aabb);

};


namespace FrustumCulling
{
    bool collidesWithPlane(glm::vec3& center, glm::vec3& extends, Plane& Plane);

}

#endif // FRUSTUM_H
