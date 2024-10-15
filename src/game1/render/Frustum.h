#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <glm/glm.hpp>

#include "../AABB.h"

struct Plane
{
    glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
    float distance = 0.f;

    Plane() { }

    /**
     * @brief Construct a new Plane object
     */
    Plane(const glm::vec3 p1, const glm::vec3 norm)
        : normal(glm::normalize(norm)), distance(glm::dot(normal, p1)) { }

    /**
     * @brief Get the Signed Distance To a Vector
     * 
     * @param point The other vector to get the distance to
     * @return The distance between both objects
     */
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

    /**
     * @brief 
     * 
     * @param aabb The other colission boxes to check against
     * @return true There is a colission
     * @return false There is not a colission
     */
    bool collidesWith(const AABB aabb);

};

namespace FrustumCulling
{
    /**
     * @brief Checks if any vectors vollide with the plane
     * 
     * @return True if there is a colission or not, false if not
     */
    bool collidesWithPlane(glm::vec3& center, glm::vec3& extends, Plane& Plane);
}

#endif // FRUSTUM_H
