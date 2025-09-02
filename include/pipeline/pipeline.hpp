#pragma once

// Header File Include //
#include "pipeline/primitives/primitive.hpp"

// STD Include //
#include <vector>

#include "skybox.hpp"

constexpr unsigned SHADOW_WIDTH = 1920;
constexpr unsigned SHADOW_HEIGHT = 2048;

class Pipeline {
    Logger _logger = Logger::getInstance();
    SkyboxPtr _skybox;
    unsigned int _cubeMapTexture;
    GLuint _depthFBO;
    unsigned _shadow = 0;

    std::vector<glm::vec3> transformAABB(const glm::mat4 &model, const glm::vec3 &localMin, const glm::vec3 &localMax);

    AABB computeWorldAABB(const glm::mat4 &modelMatrix, const AABB &box);

public:
    Pipeline();

    Pipeline(const Pipeline &) = delete;

    void initializeImgui();

    void render(const PrimitiveList &primitives, const glm::mat4 &view, const glm::mat4 &projection) const;

    ~Pipeline() = default;

    Pipeline &operator=(const Pipeline &) = delete;
};
