#pragma once

#include "pipeline/primitives/primitive.hpp"

class Plane final : public Primitive {
    static constexpr float vertices[] = {
        -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0, 1, 0, 1, 0, 0,
        1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0, 1, 0, 1, 0, 0,
        1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0, 1, 0, 1, 0, 0,
        1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0, 1, 0, 1, 0, 0,
        -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0, 1, 0, 1, 0, 0,
        -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0, 1, 0, 1, 0, 0,
    };

    unsigned _vao = 0;
    unsigned _vbo = 0;

public:
    explicit Plane();

    [[nodiscard]] AABB getLocalBox() const override;

    void render(const glm::mat4 &view, const glm::mat4 &projection) override;

    void renderDepth(const Shader &shader) override;

    void scale(float ratio) override;

    ~Plane() override;
};
