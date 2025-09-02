#pragma once

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

struct Light {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
};

class LightRepository {
public:
    static LightRepository &getInstance() {
        static LightRepository instance;

        return instance;
    }

    [[nodiscard]] Light &getDirectionalLight() {
        return _directionalLight;
    }

    [[nodiscard]] Light &getAmbientLight() {
        return _ambientLight;
    }

    [[nodiscard]] std::vector<Light> &getPointLights() {
        return _pointLights;
    }

    [[nodiscard]] Light &getSpotLight() {
        return _spotLight;
    }

    [[nodiscard]] glm::mat4 getDirectionalLightMatrix() const {
        const glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
        const glm::mat4 lightView = lookAt(_directionalLight.position,
                                           _directionalLight.position + _directionalLight.direction,
                                           glm::vec3(0.0f, 1.0f, 0.0f));

        return lightProjection * lightView;
    }

    void addPointLight(const Light& light) {
        _pointLights.push_back(light);
    }

private:
    explicit LightRepository() {
        _directionalLight = {glm::vec3(0, 10, 10), glm::vec3(0, 1, 0), glm::vec3(1.0f, 0.95f, 0.9f)};
        _ambientLight = {glm::vec3(0), glm::vec3(0), glm::vec3(0.2f, 0.2f, 0.2f)};
        _pointLights.push_back({glm::vec3(11.5f, 1.5f, 0.0f), glm::vec3(0), glm::vec3(1.0f, 1.0f, 0.0f)});
        _spotLight = {glm::vec3(0, 0, -3), glm::vec3(0, 0, 1), glm::vec3(1.0f, 1.0f, 0.0f)};
    }

    Light _directionalLight;
    Light _ambientLight;
    std::vector<Light> _pointLights;
    Light _spotLight;
};
