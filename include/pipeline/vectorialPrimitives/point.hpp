#pragma once
#include "vectorial-primitive.hpp"

class Point final : public VectorialPrimitive {
    static constexpr float vertices[] = {
        0.0f, 0.0f, 0.0f
    };

    unsigned _VAO = 0;
    unsigned _VBO = 0;

public:
    explicit Point();

    [[nodiscard]] AABB getLocalBox() const override;

    void render(const glm::mat4 &view, const glm::mat4 &projection) override;

    void renderDepth(const Shader &shader) override;

    ~Point() override;
};
