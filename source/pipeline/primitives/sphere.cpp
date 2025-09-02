#include <cmath>
#include "glad.hpp"

#include "pipeline/primitives/sphere.hpp"
#include "pipeline/shader-factory.hpp"

void Sphere::updateTopology() {
    generateMesh();

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sphere::generateMesh() {
    const float sectorStep = 2.0f * PI / static_cast<float>(_sectorCount);
    const float stackStep = PI / static_cast<float>(_stackCount);

    _vertices.clear();
    for (unsigned stackIndex = 0; stackIndex < _stackCount; ++stackIndex) {
        const float theta1 = static_cast<float>(stackIndex) * stackStep;
        const float theta2 = static_cast<float>(stackIndex + 1) * stackStep;

        for (unsigned sectorIndex = 0; sectorIndex < _sectorCount; ++sectorIndex) {
            const float phi1 = static_cast<float>(sectorIndex) * sectorStep;
            const float phi2 = static_cast<float>(sectorIndex + 1) * sectorStep;
            float x1 = _radius * std::sin(theta1) * std::cos(phi1);
            float y1 = _radius * std::cos(theta1);
            float z1 = _radius * std::sin(theta1) * std::sin(phi1);
            float x2 = _radius * std::sin(theta2) * std::cos(phi1);
            float y2 = _radius * std::cos(theta2);
            float z2 = _radius * std::sin(theta2) * std::sin(phi1);
            float x3 = _radius * std::sin(theta1) * std::cos(phi2);
            float y3 = _radius * std::cos(theta1);
            float z3 = _radius * std::sin(theta1) * std::sin(phi2);
            float x4 = _radius * std::sin(theta2) * std::cos(phi2);
            float y4 = _radius * std::cos(theta2);
            float z4 = _radius * std::sin(theta2) * std::sin(phi2);
            float u1 = static_cast<float>(sectorIndex) / static_cast<float>(_sectorCount);
            float v1 = static_cast<float>(stackIndex) / static_cast<float>(_stackCount);
            float u2 = static_cast<float>(sectorIndex) / static_cast<float>(_sectorCount);
            float v2 = static_cast<float>(stackIndex + 1) / static_cast<float>(_stackCount);
            float u3 = static_cast<float>(sectorIndex + 1) / static_cast<float>(_sectorCount);
            float v3 = static_cast<float>(stackIndex) / static_cast<float>(_stackCount);
            float u4 = static_cast<float>(sectorIndex + 1) / static_cast<float>(_sectorCount);
            float v4 = static_cast<float>(stackIndex + 1) / static_cast<float>(_stackCount);
            glm::vec3 normal1 = normalize(glm::vec3(x1, y1, z1));
            glm::vec3 normal2 = normalize(glm::vec3(x2, y2, z2));
            glm::vec3 normal3 = normalize(glm::vec3(x3, y3, z3));
            glm::vec3 normal4 = normalize(glm::vec3(x4, y4, z4));
            glm::vec3 tangent1 = normalize(glm::vec3(x2 - x1, y2 - y1, z2 - z1));
            glm::vec3 tangent2 = normalize(glm::vec3(x3 - x1, y3 - y1, z3 - z1));

            _vertices.insert(_vertices.end(), {
                                 x1, y1, z1, u1, v1, normal1.x, normal1.y, normal1.z, tangent1.x, tangent1.y, tangent1.z
                             });
            _vertices.insert(_vertices.end(), {
                                 x2, y2, z2, u2, v2, normal2.x, normal2.y, normal2.z, tangent1.x, tangent1.y, tangent1.z
                             });
            _vertices.insert(_vertices.end(), {
                                 x3, y3, z3, u3, v3, normal3.x, normal3.y, normal3.z, tangent2.x, tangent2.y, tangent2.z
                             });

            _vertices.insert(_vertices.end(), {
                                 x2, y2, z2, u2, v2, normal2.x, normal2.y, normal2.z, tangent1.x, tangent1.y, tangent1.z
                             });
            _vertices.insert(_vertices.end(), {
                                 x4, y4, z4, u4, v4, normal4.x, normal4.y, normal4.z, tangent2.x, tangent2.y, tangent2.z
                             });
            _vertices.insert(_vertices.end(), {
                                 x3, y3, z3, u3, v3, normal3.x, normal3.y, normal3.z, tangent2.x, tangent2.y, tangent2.z
                             });
        }
    }
}

Sphere::Sphere(const float radius): Primitive(ShaderFactory::getInstance().getTextureShader(),
                                              ShaderFactory::getInstance().getGlowShader()), _radius(radius) {
    generateMesh();

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void *>(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void *>(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    _properties.emplace_back(std::make_shared<PropertyCategory>("Topology"));
    _properties.emplace_back(std::make_unique<IntProperty>(SECTOR_COUNT, "Sector Count", _sectorCount,
                                                           [this](const int value) {
                                                               _sectorCount = value;
                                                               updateTopology();
                                                           }));
    _properties.emplace_back(std::make_unique<IntProperty>(STACK_COUNT, "Stack Count", _stackCount,
                                                           [this](const int value) {
                                                               _stackCount = value;
                                                               updateTopology();
                                                           }));
    _disableNormalMapping = true;
}

AABB Sphere::getLocalBox() const {
    return (AABB){
        .min = glm::vec3(-1.0f, -1.0f, -1.0f),
        .max = glm::vec3(1.0f, 1.0f, 1.0f),
    };
}

void Sphere::render(const glm::mat4 &view, const glm::mat4 &projection) {
    Primitive::render(view, projection);

    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(_vertices.size() / 11));
}

void Sphere::renderDepth(const Shader &shader) {
    Primitive::renderDepth(shader);

    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(_vertices.size() / 11));
}

Sphere::~Sphere() {
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
}
