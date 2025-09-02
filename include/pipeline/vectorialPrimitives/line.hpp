#pragma once

#include "vectorial-primitive.hpp"

class Line final : public VectorialPrimitive {
    static constexpr float vertices[] = {
        -1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f
    };

    unsigned _VAO = 0;
    unsigned _VBO = 0;

public:
    Line();

    [[nodiscard]] AABB getLocalBox() const override;

    void render(const glm::mat4 &view, const glm::mat4 &projection) override;

    void renderDepth(const Shader &shader) override;

    ~Line() override;
};
