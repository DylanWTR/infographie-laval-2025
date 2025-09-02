#include <glad.hpp>

#include "pipeline/primitives/catmull-rom-curve.hpp"
#include "pipeline/shader-factory.hpp"

CatmullRomCurve::CatmullRomCurve()
    : Primitive(ShaderFactory::getInstance().getTextureShader(),
                ShaderFactory::getInstance().getGlowShader()) {
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
}

CatmullRomCurve::~CatmullRomCurve() {
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
}

void CatmullRomCurve::setControlPoints(const std::vector<glm::vec3> &points) {
    _controlPoints = points;
    generateCurve();
}

void CatmullRomCurve::setResolution(const int resolution) {
    _resolution = resolution;
    generateCurve();
}

void CatmullRomCurve::generateCurve() {
    _vertices.clear();

    _minSize = glm::vec3(std::numeric_limits<float>::max());
    _maxSize = glm::vec3(std::numeric_limits<float>::lowest());

    if (_controlPoints.size() < 4)
        return;

    constexpr int circleSegments = 12;

    std::vector<glm::vec3> points;
    std::vector<glm::vec3> tangents;

    for (size_t i = 0; i < _controlPoints.size() - 3; ++i) {
        const glm::vec3 &P0 = _controlPoints[i];
        const glm::vec3 &P1 = _controlPoints[i + 1];
        const glm::vec3 &P2 = _controlPoints[i + 2];
        const glm::vec3 &P3 = _controlPoints[i + 3];

        for (int j = 0; j <= _resolution; ++j) {
            float t = static_cast<float>(j) / static_cast<float>(_resolution);
            float t2 = t * t;
            float t3 = t2 * t;
            glm::vec3 point = 0.5f * (2.0f * P1 + (-P0 + P2) * t +
                                      (2.0f * P0 - 5.0f * P1 + 4.0f * P2 - P3) * t2 +
                                      (-P0 + 3.0f * P1 - 3.0f * P2 + P3) * t3);
            glm::vec3 tangent = normalize(0.5f * (-P0 + P2 + 2.0f *
                                                  (2.0f * P0 - 5.0f * P1 + 4.0f * P2 - P3) * t + 3.0f *
                                                  (-P0 + 3.0f * P1 - 3.0f * P2 + P3) * t2));

            points.push_back(point);
            tangents.push_back(tangent);
        }
    }

    for (size_t i = 0; i < points.size(); ++i) {
        glm::vec3 tangent = tangents[i];
        glm::vec3 normal = normalize(glm::cross(tangent, glm::vec3(0, 1, 0)));
        if (length(normal) < 0.001f)
            normal = normalize(glm::cross(tangent, glm::vec3(1, 0, 0)));
        glm::vec3 biNormal = normalize(cross(tangent, normal));

        for (int j = 0; j <= circleSegments; ++j) {
            constexpr float radius = 0.05f;
            float theta = (j % circleSegments) * 2.0f * PI / circleSegments;
            glm::vec3 offset = normal * static_cast<float>(cos(theta)) + biNormal * static_cast<float>(sin(theta));
            glm::vec3 vertex = points[i] + radius * offset;
            glm::vec3 normalDir = normalize(offset);

            _minSize = min(vertex, _minSize);
            _maxSize = max(vertex, _maxSize);
            _vertices.insert(_vertices.end(), {
                                 vertex.x, vertex.y, vertex.z,
                                 0.0f, 0.0f,
                                 normalDir.x, normalDir.y, normalDir.z,
                                 normalDir.x, normalDir.y, normalDir.z
                             });
        }
    }

    std::vector<unsigned int> indices;
    int ringSize = circleSegments + 1;

    for (size_t i = 0; i < points.size() - 1; ++i) {
        for (int j = 0; j < circleSegments; ++j) {
            int a = i * ringSize + j;
            int b = (i + 1) * ringSize + j;
            int c = a + 1;
            int d = b + 1;

            indices.insert(indices.end(), {
                               static_cast<unsigned>(a),
                               static_cast<unsigned>(b),
                               static_cast<unsigned>(c)
                           });

            indices.insert(indices.end(), {
                               static_cast<unsigned>(c),
                               static_cast<unsigned>(b),
                               static_cast<unsigned>(d)
                           });
        }
    }

    if (_VBO == 0)
        glGenBuffers(1, &_VBO);
    if (_VAO == 0)
        glGenVertexArrays(1, &_VAO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *) (5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *) (8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    _indexCount = static_cast<unsigned>(indices.size());

    if (_EBO == 0)
        glGenBuffers(1, &_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);
}

void CatmullRomCurve::render(const glm::mat4 &view, const glm::mat4 &projection) {
    Primitive::render(view, projection);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void CatmullRomCurve::renderDepth(const Shader &shader) {
    Primitive::renderDepth(shader);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

AABB CatmullRomCurve::getLocalBox() const {
    return AABB{.min = _minSize, .max = _maxSize};
}
