#include <cmath>
#include <glad.hpp>

#include "pipeline/primitives/frustum.hpp"
#include "pipeline/shader-factory.hpp"

void Frustum::generateCap(float radius, const float sectorStep, float y) {
    const auto normal = glm::vec3(0.0f, y > 0 ? 1.0f : -1.0f, 0.0f);
    constexpr auto tangent = glm::vec3(1.0f, 0.0f, 0.0f);
    static float centerX = 0.0f;
    static float centerZ = 0.0f;
    static float uCenter = 0.5f;
    static float vCenter = 0.5f;

    for (unsigned index = 0; index < _sectorCount; ++index) {
        const float angle0 = static_cast<float>(index) * sectorStep;
        const float angle1 = static_cast<float>(index + 1) * sectorStep;
        float x0 = radius * std::cos(angle0);
        float z0 = radius * std::sin(angle0);
        float x1 = radius * std::cos(angle1);
        float z1 = radius * std::sin(angle1);
        float u0 = x0 / (radius * 2) + 0.5f;
        float v0 = z0 / (radius * 2) + 0.5f;
        float u1 = x1 / (radius * 2) + 0.5f;
        float v1 = z1 / (radius * 2) + 0.5f;

        pushVertex(glm::vec3(centerX, y, centerZ), normal, tangent, glm::vec2(uCenter, vCenter));
        pushVertex(glm::vec3(x0, y, z0), normal, tangent, glm::vec2(u0, v0));
        pushVertex(glm::vec3(x1, y, z1), normal, tangent, glm::vec2(u1, v1));
    }
}

void Frustum::updateTopology() {
    generateMesh();

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<long>(_vertices.size() * sizeof(float)), _vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Frustum::generateMesh() {
    const float sectorStep = 2.0f * PI / static_cast<float>(_sectorCount);
    const float stackHeight = _height / static_cast<float>(_stackCount);

    _vertices.clear();
    for (unsigned stackIndex = 0; stackIndex < _stackCount; ++stackIndex) {
        float y0 = -_height / 2 + static_cast<float>(stackIndex) * stackHeight;
        float y1 = y0 + stackHeight;
        const float r0 = _baseRadius + (_topRadius - _baseRadius) * (
                             static_cast<float>(stackIndex) / static_cast<float>(_stackCount));
        const float r1 = _baseRadius + (_topRadius - _baseRadius) * (
                             static_cast<float>(stackIndex + 1) / static_cast<float>(_stackCount));

        for (unsigned sectorIndex = 0; sectorIndex < _sectorCount; ++sectorIndex) {
            const float angle0 = static_cast<float>(sectorIndex) * sectorStep;
            const float angle1 = static_cast<float>(sectorIndex + 1) * sectorStep;
            float x0 = r0 * std::cos(angle0);
            float z0 = r0 * std::sin(angle0);
            float x1 = r1 * std::cos(angle0);
            float z1 = r1 * std::sin(angle0);
            float x2 = r0 * std::cos(angle1);
            float z2 = r0 * std::sin(angle1);
            float x3 = r1 * std::cos(angle1);
            float z3 = r1 * std::sin(angle1);
            float u0 = static_cast<float>(sectorIndex) / static_cast<float>(_sectorCount);
            float u1 = static_cast<float>(sectorIndex + 1) / static_cast<float>(_sectorCount);
            float v0 = static_cast<float>(stackIndex) / static_cast<float>(_stackCount);
            float v1 = static_cast<float>(stackIndex + 1) / static_cast<float>(_stackCount);
            glm::vec3 p0(x0, y0, z0);
            glm::vec3 p1(x1, y1, z1);
            glm::vec3 p2(x2, y0, z2);
            glm::vec3 p3(x3, y1, z3);
            glm::vec2 uv0(u0, v0);
            glm::vec2 uv1(u0, v1);
            glm::vec2 uv2(u1, v0);
            glm::vec2 uv3(u1, v1);
            glm::vec3 edge1 = p1 - p0;
            glm::vec3 edge2 = p2 - p0;
            glm::vec3 normal = normalize(glm::cross(edge1, edge2));
            glm::vec2 deltaUV1 = uv1 - uv0;
            glm::vec2 deltaUV2 = uv2 - uv0;
            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
            glm::vec3 tangent;

            tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
            tangent = normalize(tangent);

            pushVertex(p0, normal, tangent, uv0);
            pushVertex(p1, normal, tangent, uv1);
            pushVertex(p2, normal, tangent, uv2);

            edge1 = p3 - p1;
            edge2 = p2 - p1;
            normal = normalize(glm::cross(edge1, edge2));
            deltaUV1 = uv3 - uv1;
            deltaUV2 = uv2 - uv1;
            f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
            tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
            tangent = normalize(tangent);

            pushVertex(p1, normal, tangent, uv1);
            pushVertex(p3, normal, tangent, uv3);
            pushVertex(p2, normal, tangent, uv2);
        }
    }

    generateCap(_topRadius, sectorStep, _height / 2);
    generateCap(_baseRadius, sectorStep, -_height / 2);
}

void Frustum::pushVertex(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec3 &tangent,
                         const glm::vec2 &texture) {
    _vertices.insert(_vertices.end(), {position.x, position.y, position.z});
    _vertices.insert(_vertices.end(), {texture.x, texture.y});
    _vertices.insert(_vertices.end(), {normal.x, normal.y, normal.z});
    _vertices.insert(_vertices.end(), {tangent.x, tangent.y, tangent.z});
}

Frustum::Frustum(const float baseRadius, const float topRadius): Primitive(
                                                                     ShaderFactory::getInstance().getTextureShader(),
                                                                     ShaderFactory::getInstance().getGlowShader()),
                                                                 _baseRadius(baseRadius), _topRadius(topRadius) {
    generateMesh();

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<long>(_vertices.size() * sizeof(float)), _vertices.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void *>(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), reinterpret_cast<void *>(8 * sizeof(float)));
    glEnableVertexAttribArray(4);

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

AABB Frustum::getLocalBox() const {
    return (AABB){
        .min = glm::vec3(-1.0f, -1.0f, -1.0f),
        .max = glm::vec3(1.0f, 1.0f, 1.0f),
    };
}

void Frustum::render(const glm::mat4 &view, const glm::mat4 &projection) {
    Primitive::render(view, projection);

    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(_vertices.size() / 11));
}

void Frustum::renderDepth(const Shader &shader) {
    Primitive::renderDepth(shader);

    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(_vertices.size() / 11));
}

Frustum::~Frustum() {
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
}
