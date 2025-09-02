#pragma once

// GLM Include //
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Assimp Include //
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

// STD Include //
#include <string>
#include <vector>
#include <limits>

// Header File Include //
#include "pipeline/mesh.hpp"
#include "pipeline/primitives/primitive.hpp"
#include "application/logger.hpp"

class Model final : public Primitive {
    std::vector<Texture> _loadedTextures;
    std::vector<Mesh> _meshes;
    std::string _directory;

    glm::vec3 _minSize = glm::vec3(std::numeric_limits<float>::max());
    glm::vec3 _maxSize = glm::vec3(std::numeric_limits<float>::min());

    Logger _logger = Logger::getInstance();

    void processNode(const aiNode *node, const aiScene *scene);

    void loadMaterial(const aiMesh *mesh, const aiScene *scene, std::vector<Texture> &textures);

    Mesh processMesh(const aiMesh *mesh, const aiScene *scene);

    std::vector<Texture> loadTextures(const aiMaterial *material, aiTextureType type, const std::string_view &typeName);

public:
    explicit Model(const std::string &path);

    [[nodiscard]] AABB getLocalBox() const override;

    void render(const glm::mat4 &view, const glm::mat4 &projection) override;

    void renderDepth(const Shader &shader) override;

    void disableNormalMapping();

    ~Model() override = default;

    Model(const Model &) = delete;

    Model &operator=(const Model &) = delete;
};