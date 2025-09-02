#include <glad.hpp>

#include "pipeline/shader-factory.hpp"
#include "pipeline/primitives/cube.hpp"

Cube::Cube() : Primitive(ShaderFactory::getInstance().getTextureShader(), ShaderFactory::getInstance().getGlowShader()) {
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position //
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    // Texture //
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Normal //
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void *>(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Tangent //
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void *>(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    _disableNormalMapping = true;
}

AABB Cube::getLocalBox() const {
    return (AABB) {
        .min = glm::vec3(-1.0f, -1.0f, -1.0f),
        .max = glm::vec3(1.0f, 1.0f, 1.0f),
    };
}

void Cube::render(const glm::mat4 &view, const glm::mat4 &projection) {
    Primitive::render(view, projection);

    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Cube::renderDepth(const Shader &shader) {
    Primitive::renderDepth(shader);

    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

Cube::~Cube() {
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
}
