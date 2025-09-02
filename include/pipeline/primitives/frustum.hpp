#pragma once

#include "pipeline/primitives/primitive.hpp"

class Frustum final : public Primitive {
    unsigned _VAO = 0;
    unsigned _VBO = 0;

    float _baseRadius = 1.0f;
    float _topRadius = 1.0f;
    float _height = 2.0f;
    unsigned _sectorCount = 128;
    unsigned _stackCount = 16;

    std::vector<float> _vertices;

    IntPropertyPtr _sectorCountProperty;
    IntPropertyPtr _stackCountProperty;

    void generateCap(float radius, float sectorStep, float y);

    void updateTopology();

    void generateMesh();

    void pushVertex(const glm::vec3 &position,
                    const glm::vec3 &normal,
                    const glm::vec3 &tangent,
                    const glm::vec2 &texture);

public:
    explicit Frustum(float baseRadius, float topRadius);

    void render(const glm::mat4 &view, const glm::mat4 &projection) override;

    void renderDepth(const Shader &shader) override;

    [[nodiscard]] AABB getLocalBox() const override;

    ~Frustum() override;
};
