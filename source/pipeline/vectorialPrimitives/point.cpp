#include <glad.hpp>

#include "pipeline/vectorialPrimitives/point.hpp"
#include "pipeline/shader-factory.hpp"

Point::Point()
    : VectorialPrimitive(ShaderFactory::getInstance().getTextureShader(), ShaderFactory::getInstance().getGlowShader()) {
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

AABB Point::getLocalBox() const {
    return {getPosition(), getPosition()};
}

void Point::render(const glm::mat4& view, const glm::mat4& projection) {
    Primitive::render(view, projection);

    _shader.setVec3("color", _color);
    glBindVertexArray(_VAO);
    glDrawArrays(GL_POINTS, 0, 1);
    glBindVertexArray(0);
}

void Point::renderDepth(const Shader &shader) {
    Primitive::renderDepth(shader);

    glBindVertexArray(_VAO);
    glDrawArrays(GL_POINTS, 0, 1);
}

Point::~Point() {
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
}
