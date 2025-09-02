#pragma once

#include "pipeline/shader.hpp"

class ShaderFactory {
    Shader _texturedDepthShader = Shader("shaders/normalShader.vert", "shaders/normalShader.frag");
    Shader _depthShader = Shader("shaders/depthShader.vert", "shaders/depthShader.frag");
    Shader _glowShader = Shader("shaders/basicShader.vert", "shaders/glowShader.frag");

    explicit ShaderFactory() = default;

public:
    static ShaderFactory &getInstance() {
        static ShaderFactory instance;

        return instance;
    }

    [[nodiscard]] Shader &getTextureShader() { return _texturedDepthShader; }

    [[nodiscard]] Shader &getDepthShader() { return _depthShader; }

    [[nodiscard]] Shader &getGlowShader() { return _glowShader; }

    void operator=(ShaderFactory const &) = delete;

    ShaderFactory(ShaderFactory &) = delete;

    ShaderFactory(const ShaderFactory &) = delete;
};
