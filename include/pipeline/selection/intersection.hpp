#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "window/window.hpp"

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};

class Primitive;
class VectorialPrimitive;

using PrimitivePtr = std::shared_ptr<Primitive>;
using PrimitiveList = std::vector<PrimitivePtr>;

using VectorialPrimitivePtr = std::shared_ptr<VectorialPrimitive>;
using VectorialPrimitiveList = std::vector<VectorialPrimitivePtr>;

using Coordinates = std::pair<double, double>;

class Intersection {
    static float calculateDistance(glm::vec3 origin, glm::vec3 direction, AABB box);

    static PrimitivePtr findNearestPrimitive(const PrimitiveList &primitives, glm::vec3 origin, glm::vec3 direction);

public:
    static PrimitivePtr findNearestPrimitive(const PrimitiveList &primitives, const Window &window,
                                             const Coordinates &mouse);
};
