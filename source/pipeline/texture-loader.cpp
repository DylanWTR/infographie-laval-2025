#include <glad.hpp>
#include <cstring>

// STD Include //
#include <stb_image.hpp>

#ifndef __APPLE__
#include <GL/gl.h>
#endif

#include "pipeline/texture-loader.hpp"
#include "application/logger.hpp"
#include "exception/texture-exception.hpp"

void TextureLoader::loadImage(const char *path) {
    const Logger logger = Logger::getInstance();
    int width = 0;
    int height = 0;
    int channels = 0;
    unsigned char *data = stbi_load(path, &width, &height, &channels, 0);
    GLenum format = GL_RGB;

    if (channels == 1)
        format = GL_RED;
    else if (channels == 3)
        format = GL_RGB;
    else if (channels == 4)
        format = GL_RGBA;
    logger.debug("Load image: {} ({} channels)", path, channels);
    if (data == nullptr)
        throw TextureException("Unable to load texture " + std::string(path));
    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<int>(format), width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

unsigned TextureLoader::loadTexture(const char *texturePath) {
    unsigned textureId;

    for (const auto &[path, texture] : textureCache) {
        if (strcmp(texturePath, path) == 0)
            return texture;
    }
    glGenTextures(1, &textureId);
    stbi_set_flip_vertically_on_load(true);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    loadImage(texturePath);
    textureCache[strdup(texturePath)] = textureId;
    return textureId;
}
