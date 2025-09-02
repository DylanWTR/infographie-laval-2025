#pragma once

#include <memory>

#include "pipeline/selection/selectable.hpp"
#include "pipeline/selection/intersection.hpp"

#ifdef __APPLE__
#define PI static_cast<float>(M_PI)
#else
#define PI M_PIf
#endif

enum PrimitiveType {
    CUBE,
    PLANE,
    CYLINDER,
    CONE,
    SPHERE,
    MODEL,
    BEZIER_SURFACE,
    CATMULL_ROM_CURVE
};

constexpr std::string primitiveTypeNames[8] = {"Cube", "Plane", "Cylinder", "Cone", "Sphere", "Model", "Bezier Surface", "Catmull-Rom"};

class Primitive : public Selectable {
protected:
    Logger _logger = Logger::getInstance();
    Shader &_shader;
    unsigned _texture = 0;

    FloatPropertyPtr _positionX;
    FloatPropertyPtr _positionY;
    FloatPropertyPtr _positionZ;

    FloatPropertyPtr _rotationX;
    FloatPropertyPtr _rotationY;
    FloatPropertyPtr _rotationZ;

    FloatPropertyPtr _scaleX;
    FloatPropertyPtr _scaleY;
    FloatPropertyPtr _scaleZ;

    StringPropertyPtr _texturePath;
    BoolPropertyPtr _textureEnabled;
    IntPropertyPtr _colorR;
    IntPropertyPtr _colorG;
    IntPropertyPtr _colorB;

    FloatPropertyPtr _roughnessProperty;
    FloatPropertyPtr _metallicProperty;
    IntPropertyPtr _filterProperty;

    float _roughness = 0.99f;
    float _metallic = 0.2f;
    glm::vec3 _color = glm::vec3(0);
    int _filterType = 0;

    bool _disableNormalMapping = false;

    void initializePositionProperties();
    void initializeRotationProperties();
    void initializeScaleProperties();
    void initializeTextureProperties();
    void initializeMaterialProperties();

    void setTransformation(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

public:
    explicit Primitive(Shader &shader, Shader &glowShader);

    virtual void render(const glm::mat4 &view, const glm::mat4 &projection);

    virtual void renderDepth(const Shader &shader);

    virtual void translate(const glm::vec3 &translation);

    virtual void rotate(float degrees, const glm::vec3 &axis);

    virtual void scale(const glm::vec3 &ratio);

    virtual void scale(float ratio);

    void loadTexture(const std::string &texturePath);

    [[nodiscard]] glm::vec3 getPosition() const;

    [[nodiscard]] glm::vec3 getRotation() const;

    [[nodiscard]] glm::vec3 getScale() const;

    void setRoughness(float roughness);

    void setMetallic(float metallic);

    void setColor(glm::vec3 color);

    void applyFilter(int filterType);
};

using PrimitivePtr = std::shared_ptr<Primitive>;
using PrimitiveList = std::vector<PrimitivePtr>;
