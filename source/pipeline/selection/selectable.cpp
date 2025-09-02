#include <glad.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "pipeline/selection/selectable.hpp"

std::vector<glm::vec3> Selectable::transformLocalBox() const {
    const auto &[min, max] = getLocalBox();
    const glm::vec3 corners[8] = {
        glm::vec3(min.x, min.y, min.z),
        glm::vec3(min.x, min.y, max.z),
        glm::vec3(min.x, max.y, min.z),
        glm::vec3(min.x, max.y, max.z),
        glm::vec3(max.x, min.y, min.z),
        glm::vec3(max.x, min.y, max.z),
        glm::vec3(max.x, max.y, min.z),
        glm::vec3(max.x, max.y, max.z)
    };
    std::vector<glm::vec3> points;

    for (unsigned index = 0; index < 8; index++) {
        glm::vec4 position = _model * glm::vec4(corners[index], 1.0f);

        points.push_back(glm::vec3(position));
    }

    return points;
}

Selectable::Selectable(Shader &shader) : _shader(shader) {
    const auto indicesLength = static_cast<long>(_indices.size() * sizeof(unsigned));

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 8, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesLength, _indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Selectable::renderSelectionBox(const glm::mat4 &view, const glm::mat4 &projection) const {
    const auto &[min, max] = getLocalBox();
    const std::vector<glm::vec3> vertices = {
        {min.x, min.y, min.z}, {max.x, min.y, min.z}, {max.x, max.y, min.z}, {min.x, max.y, min.z},
        {min.x, min.y, max.z}, {max.x, min.y, max.z}, {max.x, max.y, max.z}, {min.x, max.y, max.z}
    };

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<long>(sizeof(glm::vec3) * vertices.size()), vertices.data());

    _shader.use();
    _shader.setMat4("view", view);
    _shader.setMat4("projection", projection);
    _shader.setMat4("model", _model);
    _shader.setVec3("color", glm::vec3(1.0, 0.0, 0.0));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glDepthMask(GL_FALSE);
    glDepthFunc(GL_ALWAYS);

    glLineWidth(10.0f);

    glBindVertexArray(_VAO);
    glDrawElements(GL_LINES, static_cast<int>(_indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glDepthFunc(GL_LESS);
}

std::vector<PropertyPtr> Selectable::getProperties() const {
    return _properties;
}

AABB Selectable::getCollisionBox() const {
    std::vector<glm::vec3> points = transformLocalBox();
    glm::vec3 worldMin = points[0];
    glm::vec3 worldMax = points[0];

    for (const glm::vec3 &point: points) {
        worldMin = min(worldMin, point);
        worldMax = max(worldMax, point);
    }
    return {worldMin, worldMax};
}

Selectable::~Selectable() {
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
}
