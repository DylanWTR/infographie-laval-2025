#pragma once

#include "pipeline/primitives/primitive.hpp"

class Sphere final : public Primitive {
    unsigned _VAO = 0;
    unsigned _VBO = 0;

    float _radius = 1.0f;
    unsigned _sectorCount = 128;
    unsigned _stackCount = 128;

    std::vector<float> _vertices;

    IntPropertyPtr _sectorCountProperty;
    IntPropertyPtr _stackCountProperty;

    void updateTopology();

    void generateMesh();

public:
    explicit Sphere(float radius);

    void render(const glm::mat4 &view, const glm::mat4 &projection) override;

    void renderDepth(const Shader &shader) override;

    [[nodiscard]] AABB getLocalBox() const override;

    ~Sphere() override;
};
