#include <glad.hpp>

// Header File Include //
#include "application/light-repository.hpp"
#include "pipeline/pipeline.hpp"
#include "pipeline/shader-factory.hpp"
#include "pipeline/primitives/cube.hpp"

// GLFW Include //
#include <GLFW/glfw3.h>

const std::array<std::string, 6> faces{
    "resources/skybox/px.png",
    "resources/skybox/nx.png",
    "resources/skybox/py.png",
    "resources/skybox/ny.png",
    "resources/skybox/pz.png",
    "resources/skybox/nz.png",
};

Pipeline::Pipeline() {
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        _logger.critical("Failed to initialize GLAD.");
        return;
    }
    _logger.info("OpenGL version: {}", reinterpret_cast<const char *>(glGetString(GL_VERSION)));
    glEnable(GL_DEPTH_TEST);

    _skybox = std::make_unique<Skybox>();
    _cubeMapTexture = Skybox::loadCubeMap(faces);

    constexpr float borderColor[] = {1.0, 1.0, 1.0, 1.0};

    glGenFramebuffers(1, &_depthFBO);

    glGenTextures(1, &_shadow);
    glBindTexture(GL_TEXTURE_2D, _shadow);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                 nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Attache à un framebuffer sans color buffer
    glBindFramebuffer(GL_FRAMEBUFFER, _depthFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _shadow, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Pipeline::render(const PrimitiveList &primitives, const glm::mat4 &view, const glm::mat4 &projection) const {
    constexpr float radius = 10.0f;
    const double time = glfwGetTime();
    const auto directionalLightPosition = glm::vec3(radius * cos(time), 10.0f, radius * sin(time));
    const Shader &depthShader = ShaderFactory::getInstance().getDepthShader();
    LightRepository &lightRepository = LightRepository::getInstance();
    Light &directionalLight = lightRepository.getDirectionalLight();
    Light &spotLight = lightRepository.getSpotLight();

    directionalLight.position = directionalLightPosition;
    directionalLight.direction = normalize(-directionalLightPosition);

    spotLight.position = glm::vec3(inverse(view)[3]);
    spotLight.direction = glm::vec3(view[0][2], view[1][2], view[2][2]);

    depthShader.use();
    depthShader.setMat4("lightSpaceMatrix", lightRepository.getDirectionalLightMatrix());

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, _depthFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    for (auto &primitive: primitives)
        primitive->renderDepth(depthShader);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);

    glViewport(0, 0, Window::WIDTH, Window::HEIGHT);

    _skybox->render(_cubeMapTexture, glm::mat4(glm::mat3(view)), projection);

    glActiveTexture(GL_TEXTURE30);
    glBindTexture(GL_TEXTURE_2D, _shadow);
    glActiveTexture(GL_TEXTURE31);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapTexture);
    for (const auto &primitive: primitives)
        primitive->render(view, projection);
}
