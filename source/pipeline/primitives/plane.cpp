#include <glad.hpp>

#include "pipeline/shader-factory.hpp"
#include "pipeline/primitives/plane.hpp"

Plane::Plane(): Primitive(ShaderFactory::getInstance().getTextureShader(), ShaderFactory::getInstance().getGlowShader()) {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    // TexCoord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void *>(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Tangent
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void *>(8 * sizeof(float)));
    glEnableVertexAttribArray(3);
}

AABB Plane::getLocalBox() const {
    return (AABB) {
        .min = glm::vec3(-1.0f, 0.0f, -1.0f),
        .max = glm::vec3(1.0f, 0.0f, 1.0f),
    };
}

void Plane::render(const glm::mat4 &view, const glm::mat4 &projection) {
    Primitive::render(view, projection);

    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Plane::renderDepth(const Shader &shader) {
    Primitive::renderDepth(shader);

    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Plane::scale(const float ratio) {
    Primitive::scale(ratio);
}

Plane::~Plane() {
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
}
