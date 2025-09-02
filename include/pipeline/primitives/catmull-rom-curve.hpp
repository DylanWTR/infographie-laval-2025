#pragma once

#include "pipeline/primitives/primitive.hpp"

class CatmullRomCurve final : public Primitive {
    unsigned _VAO = 0;
    unsigned _VBO = 0;

    unsigned _EBO = 0;
    unsigned _indexCount = 0;

    glm::vec3 _minSize = glm::vec3(std::numeric_limits<float>::max());
    glm::vec3 _maxSize = glm::vec3(std::numeric_limits<float>::lowest());

    std::vector<glm::vec3> _controlPoints;
    std::vector<float> _vertices;
    int _resolution = 20;

    void generateCurve();

public:
    CatmullRomCurve();

    void setControlPoints(const std::vector<glm::vec3>& points);
    void setResolution(int resolution);

    void render(const glm::mat4 &view, const glm::mat4 &projection) override;
    void renderDepth(const Shader &shader) override;

    [[nodiscard]] AABB getLocalBox() const override;

    ~CatmullRomCurve() override;
};
