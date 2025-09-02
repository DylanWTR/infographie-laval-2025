// Glad Include //
#include <glad.hpp>
#include <assimp/postprocess.h>

// Header File Include //
#include "pipeline/model.hpp"
#include "exception/model-exception.hpp"
#include "pipeline/texture-loader.hpp"
#include "pipeline/shader-factory.hpp"

Model::Model(const std::string &path) : Primitive(ShaderFactory::getInstance().getTextureShader(),
                                                  ShaderFactory::getInstance().getGlowShader()) {
    Assimp::Importer importer;
    constexpr int flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
                          aiProcess_CalcTangentSpace;
    const aiScene *scene = importer.ReadFile(path, flags);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        throw ModelException(importer.GetErrorString());

    std::erase_if(_properties, [](const PropertyPtr &property) {
        return (property->type() == CATEGORY && property->name() == "> Texture") || property->type() == TEXTURE;
    });
    _directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
    _textureEnabled->updateValue(true);
}

void Model::render(const glm::mat4 &view, const glm::mat4 &projection) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    Primitive::render(view, projection);
    _shader.setBool("disableNormalMapping", false);

    if (_textureEnabled->value())
        _shader.setVec3("color", glm::vec3(-1));
    for (const auto &mesh: _meshes)
        mesh.draw(_shader, _textureEnabled->value());
    glDisable(GL_CULL_FACE);
}

void Model::renderDepth(const Shader &shader) {
    Primitive::renderDepth(shader);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    for (const auto &mesh: _meshes)
        mesh.draw(_shader, false);
    glDisable(GL_CULL_FACE);
}

void Model::processNode(const aiNode *node, const aiScene *scene) {
    for (unsigned index = 0; index < node->mNumMeshes; index++) {
        const aiMesh *mesh = scene->mMeshes[node->mMeshes[index]];

        _meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned index = 0; index < node->mNumChildren; index++)
        processNode(node->mChildren[index], scene);
}

Mesh Model::processMesh(const aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned> indices;
    std::vector<Texture> textures;
    glm::vec3 minBounds(std::numeric_limits<float>::max());
    glm::vec3 maxBounds(std::numeric_limits<float>::lowest());

    for (unsigned index = 0; index < mesh->mNumVertices; index++) {
        Vertex vertex = {};
        glm::vec3 position(
            mesh->mVertices[index].x,
            mesh->mVertices[index].y,
            mesh->mVertices[index].z
        );
        vertex.position = position;

        minBounds = min(minBounds, position);
        maxBounds = max(maxBounds, position);

        if (mesh->HasNormals()) {
            vertex.normal = glm::vec3(
                mesh->mNormals[index].x,
                mesh->mNormals[index].y,
                mesh->mNormals[index].z
            );
        } else {
            vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f);
        }

        if (mesh->mTextureCoords[0]) {
            vertex.textureCoordinates = glm::vec2(
                mesh->mTextureCoords[0][index].x,
                mesh->mTextureCoords[0][index].y
            );
        } else {
            vertex.textureCoordinates = glm::vec2(0.0f, 0.0f);
        }

        if (mesh->HasTangentsAndBitangents()) {
            vertex.tangent = glm::vec3(
                mesh->mTangents[index].x,
                mesh->mTangents[index].y,
                mesh->mTangents[index].z
            );
            vertex.biTangent = glm::vec3(
                mesh->mBitangents[index].x,
                mesh->mBitangents[index].y,
                mesh->mBitangents[index].z
            );
        } else {
            vertex.tangent = glm::vec3(0.0f, 0.0f, 0.0f);
            vertex.biTangent = glm::vec3(0.0f, 0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned index = 0; index < mesh->mNumFaces; index++) {
        const aiFace face = mesh->mFaces[index];
        if (face.mNumIndices == 3) {
            indices.push_back(face.mIndices[0]);
            indices.push_back(face.mIndices[1]);
            indices.push_back(face.mIndices[2]);
        }
    }

    loadMaterial(mesh, scene, textures);
    _minSize = min(_minSize, minBounds);
    _maxSize = max(_maxSize, maxBounds);
    return {vertices, indices, textures};
}

void Model::loadMaterial(const aiMesh *mesh, const aiScene *scene, std::vector<Texture> &textures) {
    const aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    std::vector<Texture> specularMaps = loadTextures(material, aiTextureType_SPECULAR, "texture_specular");
    std::vector<Texture> normalMaps = loadTextures(material, aiTextureType_HEIGHT, "texture_normal");
    std::vector<Texture> heightMaps = loadTextures(material, aiTextureType_AMBIENT, "texture_height");

    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
}


std::vector<Texture> Model::loadTextures(const aiMaterial *material, const aiTextureType type,
                                         const std::string_view &typeName) {
    std::vector<Texture> textures;

    for (unsigned index = 0; index < material->GetTextureCount(type); index++) {
        aiString str;
        bool skip = false;
        Texture texture;

        material->GetTexture(type, index, &str);
        for (auto &_loadedTexture: _loadedTextures) {
            if (std::strcmp(_loadedTexture.path.data(), str.C_Str()) == 0) {
                textures.push_back(_loadedTexture);
                skip = true;
                break;
            }
        }
        if (skip) continue;
        texture.id = TextureLoader::loadTexture(
            (std::string(_directory) + "/" + std::string(str.C_Str())).c_str());
        texture.type = typeName;
        texture.path = str.C_Str();
        textures.push_back(texture);
        _loadedTextures.push_back(texture);
    }
    return textures;
}

AABB Model::getLocalBox() const {
    return (AABB){
        .min = _minSize,
        .max = _maxSize,
    };
}
