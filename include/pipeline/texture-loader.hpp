#pragma once

#include <map>

static std::map<const char *, unsigned> textureCache;

class TextureLoader {
    static void loadImage(const char *path);

public:
    explicit TextureLoader() = default;

    static unsigned loadTexture(const char *texturePath);
};
