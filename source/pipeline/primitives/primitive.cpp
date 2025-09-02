#include <glad.hpp>

#include <glm/gtc/quaternion.hpp>

#include "application/light-repository.hpp"
#include "exception/texture-exception.hpp"
#include "pipeline/primitives/primitive.hpp"

#include "application/menu/scene-menu.hpp"
#include "pipeline/texture-loader.hpp"

void Primitive::initializePositionProperties() {
    _positionX = std::make_shared<FloatProperty>(POSITION, "X", 0.0f, [this](const float value) {
        const auto position = getPosition();

        setTransformation(glm::vec3(value, position[1], position[2]), getRotation(), getScale());
    });
    _positionY = std::make_shared<FloatProperty>(POSITION, "Y", 0.0f, [this](const float value) {
        const auto position = getPosition();

        setTransformation(glm::vec3(position[0], value, position[2]), getRotation(), getScale());
    });
    _positionZ = std::make_shared<FloatProperty>(POSITION, "Z", 0.0f, [this](const float value) {
        const auto position = getPosition();

        setTransformation(glm::vec3(position[0], position[1], value), getRotation(), getScale());
    });

    _properties.emplace_back(std::make_shared<PropertyCategory>("Position"));
    _properties.emplace_back(_positionX);
    _properties.emplace_back(_positionY);
    _properties.emplace_back(_positionZ);
}

void Primitive::initializeRotationProperties() {
    _rotationX = std::make_shared<FloatProperty>(ROTATION, "X", 0.0f, [this](const float value) {
        const auto rotation = getRotation();

        setTransformation(getPosition(), glm::vec3(value, rotation[1], rotation[2]), getScale());
    });
    _rotationY = std::make_shared<FloatProperty>(ROTATION, "Y", 0.0f, [this](const float value) {
        const auto rotation = getRotation();

        setTransformation(getPosition(), glm::vec3(rotation[0], value, rotation[2]), getScale());
    });
    _rotationZ = std::make_shared<FloatProperty>(ROTATION, "Z", 0.0f, [this](const float value) {
        const auto rotation = getRotation();

        setTransformation(getPosition(), glm::vec3(rotation[0], rotation[1], value), getScale());
    });

    _properties.emplace_back(std::make_shared<PropertyCategory>("Rotation"));
    _properties.emplace_back(_rotationX);
    _properties.emplace_back(_rotationY);
    _properties.emplace_back(_rotationZ);
}

void Primitive::initializeScaleProperties() {
    _scaleX = std::make_shared<FloatProperty>(SCALE, "X", 1.0f, [this](const float value) {
        const auto scale = getScale();

        setTransformation(getPosition(), getRotation(), glm::vec3(value, scale[1], scale[2]));
    });
    _scaleY = std::make_shared<FloatProperty>(SCALE, "Y", 1.0f, [this](const float value) {
        const auto scale = getScale();

        setTransformation(getPosition(), getRotation(), glm::vec3(scale[0], value, scale[2]));
    });
    _scaleZ = std::make_shared<FloatProperty>(SCALE, "Z", 1.0f, [this](const float value) {
        const auto scale = getScale();

        setTransformation(getPosition(), getRotation(), glm::vec3(scale[0], scale[1], value));
    });

    _properties.emplace_back(std::make_shared<PropertyCategory>("Scale"));
    _properties.emplace_back(_scaleX);
    _properties.emplace_back(_scaleY);
    _properties.emplace_back(_scaleZ);
}

void Primitive::initializeTextureProperties() {
    _texturePath = std::make_shared<StringProperty>(TEXTURE, "Path", "", [this](const std::string &value) {
        if (value.empty()) {
            _texture = 0;
            return;
        }
        try {
            _texture = TextureLoader::loadTexture(value.c_str());
        } catch (TextureException &exception) {
            _logger.warn("{}", exception.what());
            _texture = TextureLoader::loadTexture("resources/textures/default.png");
        }
    });
    _textureEnabled = std::make_shared<BoolProperty>(TEXTURE_ENABLED, "Texture Enabled", false, [](const bool &) {
        // Value already managed by the property reference
    });

    _colorR = std::make_shared<IntProperty>(COLOR, "R", _color.r, [this](const int value) {
        _color.r = static_cast<float>(value) / 255.0f;
    });
    _colorG = std::make_shared<IntProperty>(COLOR, "G", _color.g, [this](const int value) {
        _color.g = static_cast<float>(value) / 255.0f;
    });
    _colorB = std::make_shared<IntProperty>(COLOR, "B", _color.b, [this](const int value) {
        _color.b = static_cast<float>(value) / 255.0f;
    });

    _properties.emplace_back(std::make_shared<PropertyCategory>("Texture"));
    _properties.emplace_back(_texturePath);
    _properties.emplace_back(_textureEnabled);
    _properties.emplace_back(_colorR);
    _properties.emplace_back(_colorG);
    _properties.emplace_back(_colorB);
}

void Primitive::initializeMaterialProperties() {
    _roughnessProperty = std::make_shared<FloatProperty>(ROUGHNESS, "Roughness", _roughness, [this](const float value) {
        _roughness = value;
    });
    _metallicProperty = std::make_shared<FloatProperty>(METALLIC, "Metallic", _metallic, [this](const float value) {
        _metallic = value;
    });
    _filterProperty = std::make_shared<IntProperty>(FILTER, "Filter", _filterType, [this](const int value) {
        _filterType = value;
    });

    _properties.emplace_back(std::make_shared<PropertyCategory>("Material"));
    _properties.emplace_back(_roughnessProperty);
    _properties.emplace_back(_metallicProperty);
    _properties.emplace_back(_filterProperty);
}

void Primitive::setTransformation(const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 scale) {
    _model = glm::translate(glm::mat4(1.0f), position)
             * mat4_cast(glm::quat(radians(rotation)))
             * glm::scale(glm::mat4(1.0f), scale);
}

Primitive::Primitive(Shader &shader, Shader &glowShader) : Selectable(glowShader), _shader(shader) {
    initializePositionProperties();
    initializeRotationProperties();
    initializeScaleProperties();
    initializeTextureProperties();
    initializeMaterialProperties();
}

void Primitive::render(const glm::mat4 &view, const glm::mat4 &projection) {
    const auto cameraPosition = glm::vec3(inverse(view)[3]);

    LightRepository &lightRepository = LightRepository::getInstance();
    const Light directionalLight = lightRepository.getDirectionalLight();
    const Light ambientLight = lightRepository.getAmbientLight();
    const Light spotLight = lightRepository.getSpotLight();
    const std::vector<Light> pointLights = lightRepository.getPointLights();

    _shader.use();
    _shader.setMat4("projection", projection);
    _shader.setMat4("view", view);
    _shader.setMat4("model", _model);
    _shader.setVec3("cameraPosition", cameraPosition);

    _shader.setFloat("roughness", _roughness);
    _shader.setFloat("metallic", _metallic);
    _shader.setFloat("reflectionStrength", 0.5f);
    _shader.setBool("disableNormalMapping", true);
    _shader.setMat4("lightSpaceMatrix", lightRepository.getDirectionalLightMatrix());
    _shader.setInt("shadowMap", 30);
    _shader.setInt("skybox", 31);
    _shader.setVec3("color", _color);
    _shader.setInt("filterType", _filterType);

    // Directional Light
    _shader.setVec3("lightDir", directionalLight.direction);
    _shader.setVec3("lightColor", directionalLight.color);
    _shader.setVec3("ambientColor", ambientLight.color);

    // Spot Light
    _shader.setVec3("spotPos", spotLight.position);
    _shader.setVec3("spotDir", spotLight.direction);
    _shader.setFloat("cutOff", glm::cos(glm::radians(3.0f)));
    _shader.setFloat("outerCutOff", glm::cos(glm::radians(5.0f)));

    // Point Light
    _shader.setInt("numPointLights", pointLights.size());
    for (size_t i = 0; i < pointLights.size(); i++) {
        const auto& light = pointLights[i];
        std::string prefix = "pointLights[" + std::to_string(i) + "]";
        _shader.setVec3(prefix + ".position", light.position);
        _shader.setVec3(prefix + ".color", light.color);
        _shader.setFloat(prefix + ".constant", 1.0f);
        _shader.setFloat(prefix + ".linear", 0.09f);
        _shader.setFloat(prefix + ".quadratic", 0.032f);
    }

    auto &[enableToneMapping, toneMappingExposure] = ToneMappingSettings::instance();
    _shader.setBool("enableToneMapping", enableToneMapping);
    _shader.setFloat("toneMappingExposure", toneMappingExposure);

    if (_texture != 0 && _textureEnabled->value()) {
        glActiveTexture(GL_TEXTURE1);
        _shader.setInt("texture_diffuse1", 1);
        _shader.setVec3("color", glm::vec3(-1));
    }
    glBindTexture(GL_TEXTURE_2D, _texture);
}

void Primitive::renderDepth(const Shader &shader) {
    shader.setMat4("model", _model);
}

void Primitive::translate(const glm::vec3 &translation) {
    _model = glm::translate(_model, translation);
    glm::vec3 position = getPosition();

    _positionX->updateValue(position[0]);
    _positionY->updateValue(position[1]);
    _positionZ->updateValue(position[2]);
}

void Primitive::rotate(const float degrees, const glm::vec3 &axis) {
    _model = glm::rotate(_model, glm::radians(degrees), axis);
}

void Primitive::scale(const glm::vec3 &ratio) {
    _model = glm::scale(_model, ratio);
}

void Primitive::scale(const float ratio) {
    _model = glm::scale(_model, glm::vec3(ratio, ratio, ratio));
}

void Primitive::loadTexture(const std::string &texturePath) {
    _texture = TextureLoader::loadTexture(texturePath.c_str());
    _texturePath->updateValue(texturePath);
    _textureEnabled->updateValue(true);
}

glm::vec3 Primitive::getPosition() const {
    return {_model[3]};
}

glm::vec3 Primitive::getRotation() const {
    const glm::vec3 scale = getScale();
    glm::mat3 rotationMatrix;

    rotationMatrix[0] = glm::vec3(_model[0]) / scale.x;
    rotationMatrix[1] = glm::vec3(_model[1]) / scale.y;
    rotationMatrix[2] = glm::vec3(_model[2]) / scale.z;
    return degrees(eulerAngles(quat_cast(rotationMatrix)));
}

glm::vec3 Primitive::getScale() const {
    glm::vec3 scale;

    scale.x = length(glm::vec3(_model[0]));
    scale.y = length(glm::vec3(_model[1]));
    scale.z = length(glm::vec3(_model[2]));
    return scale;
}

void Primitive::setRoughness(const float roughness) {
    _roughness = roughness;
    _roughnessProperty->updateValue(roughness);
}

void Primitive::setMetallic(const float metallic) {
    _metallic = metallic;
    _metallicProperty->updateValue(metallic);
}

void Primitive::setColor(const glm::vec3 color) {
    _color = color;
    _colorR->updateValue(static_cast<int>(255 * color.r));
    _colorG->updateValue(static_cast<int>(255 * color.g));
    _colorB->updateValue(static_cast<int>(255 * color.b));
    _texture = 0;
}

void Primitive::applyFilter(const int filterType) {
    _filterType = filterType;
}
