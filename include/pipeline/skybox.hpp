#pragma once

#include "pipeline/shader.hpp"

#include <array>
#include <memory>

class Skybox {
public:
    explicit Skybox();

    void render(unsigned int cubeMapTexture, const glm::mat4 & view, const glm::mat4 &projection) const;

    static unsigned int loadCubeMap(const std::array<std::string, 6> &faces);

private:
    unsigned int _vao = 0;
    unsigned int _vbo = 0;
    Shader _shader;
};

using SkyboxPtr = std::unique_ptr<Skybox>;