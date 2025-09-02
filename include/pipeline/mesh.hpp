#pragma once

// Header File Include //
#include "pipeline/shader.hpp"

// GLM Include //
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// STD Include //
#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoordinates;
    glm::vec3 tangent;
    glm::vec3 biTangent;
    int boneIDs[MAX_BONE_INFLUENCE];
    float weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned id;
    std::string type;
    std::string path;
};

class Mesh {
    unsigned _VBO = 0;
    unsigned _EBO = 0;
    unsigned _VAO = 0;
    std::vector<Vertex> _vertices;
    std::vector<unsigned> _indices;
    std::vector<Texture> _textures;

public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned> &indices, const std::vector<Texture> &textures);

    void draw(const Shader &shader, bool textureEnabled) const;

    ~Mesh() = default;
};
