#pragma once

#include "pipeline/primitives/primitive.hpp"

class BezierSurface final : public Primitive {
    unsigned _VAO = 0;
    unsigned _VBO = 0;

    int resolutionU = 20;
    int resolutionV = 20;

    glm::vec3 _controlPoints[4][4];
    std::vector<float> _vertices;

    glm::vec3 _minSize = glm::vec3(std::numeric_limits<float>::max());
    glm::vec3 _maxSize = glm::vec3(std::numeric_limits<float>::lowest());

    void generateMesh();
    glm::vec3 evaluate(float u, float v) const;
    glm::vec3 bernstein(float t, const glm::vec3* points) const;

public:
    BezierSurface();
    [[nodiscard]] AABB getLocalBox() const override;

    void setResolution(int u, int v);

    void render(const glm::mat4& view, const glm::mat4& projection) override;
    void renderDepth(const Shader& shader) override;
    ~BezierSurface() override;
};
