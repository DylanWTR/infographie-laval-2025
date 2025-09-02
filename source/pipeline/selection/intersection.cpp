#include "pipeline/selection/intersection.hpp"
#include "pipeline/primitives/primitive.hpp"

float Intersection::calculateDistance(glm::vec3 origin, glm::vec3 direction, AABB box) {
    glm::vec3 inverseDirection = 1.0f / direction;
    glm::vec3 tMin = (box.min - origin) * inverseDirection;
    glm::vec3 tMax = (box.max - origin) * inverseDirection;
    glm::vec3 t1 = min(tMin, tMax);
    glm::vec3 t2 = max(tMin, tMax);
    float tNear = glm::max(glm::max(t1.x, t1.y), t1.z);
    float tFar = glm::min(glm::min(t2.x, t2.y), t2.z);

    return tNear <= tFar && tFar > 0.0f ? tNear : -1;
}

PrimitivePtr Intersection::findNearestPrimitive(const PrimitiveList &primitives, const glm::vec3 origin,
                                                const glm::vec3 direction) {
    PrimitivePtr nearestPrimitive = nullptr;
    float nearestDistance = std::numeric_limits<float>::max();

    for (const auto &primitive: primitives) {
        const float distance = calculateDistance(origin, direction, primitive->getCollisionBox());

        if (distance == -1 || distance > nearestDistance)
            continue;
        nearestPrimitive = primitive;
        nearestDistance = distance;
    }
    return nearestPrimitive;
}

PrimitivePtr Intersection::findNearestPrimitive(const PrimitiveList &primitives, const Window &window,
                                                const Coordinates &mouse) {
    const float x = (2.0f * static_cast<float>(mouse.first)) / static_cast<float>(window.getWidth()) - 1.0f;
    const float y = 1.0f - (2.0f * static_cast<float>(mouse.second)) / static_cast<float>(window.getHeight());

    if (window.getProjectionMode() == ORTHOGRAPHIC) {
        glm::vec4 originClip(x, y, -1.0f, 1.0f);
        glm::vec4 originEye = inverse(window.getProjection()) * originClip;

        originEye /= originEye.w;
        auto originWorld = glm::vec3(inverse(window.getView()) * originEye);
        auto rayWorld = glm::vec3(inverse(window.getView()) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));

        return findNearestPrimitive(primitives, originWorld, rayWorld);
    }

    const glm::vec3 rayNds(x, y, -1.0);
    const glm::vec4 rayClip(rayNds.x, rayNds.y, -1.0f, 1.0f);
    glm::vec4 rayEye = inverse(window.getProjection()) * rayClip;

    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
    return findNearestPrimitive(primitives, window.getCameraPosition(),
                                normalize(glm::vec3(inverse(window.getView()) * rayEye)));
}
