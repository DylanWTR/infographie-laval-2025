#include <glad.hpp>

#include "pipeline/primitives/bezier_surface.hpp"
#include "pipeline/shader-factory.hpp"

BezierSurface::BezierSurface()
    : Primitive(ShaderFactory::getInstance().getTextureShader(),
                ShaderFactory::getInstance().getGlowShader()) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            _controlPoints[i][j] = glm::vec3(i - 1.5f, sin(i + j), j - 1.5f);

    generateMesh();

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void *>(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void *>(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    _disableNormalMapping = true;
}

BezierSurface::~BezierSurface() {
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
}

glm::vec3 BezierSurface::bernstein(const float t, const glm::vec3 *points) const {
    const float it = 1 - t;

    return points[0] * (it * it * it) +
           points[1] * (3 * it * it * t) +
           points[2] * (3 * it * t * t) +
           points[3] * (t * t * t);
}

glm::vec3 BezierSurface::evaluate(float u, float v) const {
    glm::vec3 temp[4];
    for (int i = 0; i < 4; ++i)
        temp[i] = bernstein(v, _controlPoints[i]);
    return bernstein(u, temp);
}

void BezierSurface::generateMesh() {
    _vertices.clear();

    _minSize = glm::vec3(std::numeric_limits<float>::max());
    _maxSize = glm::vec3(std::numeric_limits<float>::lowest());

    for (int i = 0; i < resolutionU; ++i) {
        float u = static_cast<float>(i) / (resolutionU - 1);
        float nextU = static_cast<float>(i + 1) / (resolutionU - 1);

        for (int j = 0; j < resolutionV; ++j) {
            float v = static_cast<float>(j) / (resolutionV - 1);
            float nextV = static_cast<float>(j + 1) / (resolutionV - 1);

            glm::vec3 p1 = evaluate(u, v);
            glm::vec3 p2 = evaluate(nextU, v);
            glm::vec3 p3 = evaluate(u, nextV);
            glm::vec3 p4 = evaluate(nextU, nextV);

            glm::vec3 n1 = normalize(cross(p2 - p1, p3 - p1));
            glm::vec3 n2 = normalize(cross(p4 - p2, p1 - p2));
            glm::vec3 tangent = normalize(p2 - p1);

            auto push = [&](const glm::vec3 &point, float uCoord, float vCoord, const glm::vec3 &normal) {
                _minSize = min(_minSize, point);
                _maxSize = max(_maxSize, point);

                _vertices.insert(_vertices.end(), {
                                     point.x, point.y, point.z,
                                     uCoord, vCoord,
                                     normal.x, normal.y, normal.z,
                                     tangent.x, tangent.y, tangent.z
                                 });
            };

            push(p1, u, v, n1);
            push(p2, nextU, v, n1);
            push(p3, u, nextV, n1);

            push(p2, nextU, v, n2);
            push(p4, nextU, nextV, n2);
            push(p3, u, nextV, n2);
        }
    }
}

void BezierSurface::render(const glm::mat4 &view, const glm::mat4 &projection) {
    Primitive::render(view, projection);

    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(_vertices.size() / 11));
}

void BezierSurface::renderDepth(const Shader &shader) {
    Primitive::renderDepth(shader);
    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(_vertices.size() / 11));
}

AABB BezierSurface::getLocalBox() const {
    return {
        .min = _minSize,
        .max = _maxSize
    };
}

void BezierSurface::setResolution(const int u, int v) {
    resolutionU = u;
    resolutionV = v;
    generateMesh();

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW);
}
