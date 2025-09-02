#pragma once

#include <vector>

#include "application/property.hpp"
#include "pipeline/shader.hpp"
#include "pipeline/selection/intersection.hpp"

class Selectable {
    const std::vector<unsigned int> _indices = {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7
    };

    Shader &_shader;
    unsigned _VAO = 0;
    unsigned _VBO = 0;
    unsigned _EBO = 0;

    [[nodiscard]] std::vector<glm::vec3> transformLocalBox() const;

protected:
    glm::mat4 _model = glm::mat4(1.0f);
    std::vector<PropertyPtr> _properties;

public:
    explicit Selectable(Shader &shader);

    void renderSelectionBox(const glm::mat4 &view, const glm::mat4 &projection) const;

    [[nodiscard]] std::vector<PropertyPtr> getProperties() const;

    [[nodiscard]] AABB getCollisionBox() const;

    [[nodiscard]] virtual AABB getLocalBox() const = 0;

    virtual ~Selectable();
};
