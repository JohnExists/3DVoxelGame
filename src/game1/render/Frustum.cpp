#include "Frustum.h"

float Plane::getSignedDistanceTo(glm::vec3 point)
{
    return glm::dot(normal, point) - distance;
}

bool Frustum::collidesWith(const AABB aabb)
{
    using namespace FrustumCulling;
    glm::vec3 extends(aabb.size.x / 2, aabb.size.y / 2, aabb.size.z / 2);
    glm::vec3 center = aabb.position + extends;

    return collidesWithPlane(center, extends, leftFace) &&
        collidesWithPlane(center, extends, rightFace) &&
        collidesWithPlane(center, extends, topFace) &&
        collidesWithPlane(center, extends, bottomFace) &&
        collidesWithPlane(center, extends, nearFace) &&
        collidesWithPlane(center, extends, farFace);
        
}

bool FrustumCulling::collidesWithPlane(glm::vec3& center, glm::vec3& extends, Plane& plane)
{
    const float r = extends.x * std::abs(plane.normal.x) +
        extends.y * std::abs(plane.normal.y) + extends.z * std::abs(plane.normal.z);

    return -r <= plane.getSignedDistanceTo(center);
}
