#include <glad.hpp>

#include "pipeline/vectorialPrimitives/rectangle.hpp"
#include "pipeline/shader-factory.hpp"

Rectangle::Rectangle()
    : VectorialPrimitive(ShaderFactory::getInstance().getTextureShader(), ShaderFactory::getInstance().getGlowShader()) {
    constexpr unsigned indices[6] = {0, 1, 2, 2, 3, 0};

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

AABB Rectangle::getLocalBox() const {
    return {glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f)};
}

void Rectangle::render(const glm::mat4 &view, const glm::mat4 &projection) {
    Primitive::render(view, projection);

    _shader.setVec3("color", _color);
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Rectangle::renderDepth(const Shader &shader) {
    Primitive::renderDepth(shader);

    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

Rectangle::~Rectangle() {
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
}
