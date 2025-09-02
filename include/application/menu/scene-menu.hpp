#pragma once

#include <memory>

struct ToneMappingSettings {
    bool  enableToneMapping = false;
    float toneMappingExposure = 1.0f;

    static ToneMappingSettings &instance() {
        static ToneMappingSettings settings;
        return settings;
    }
};

class SceneMenu {
public:
    SceneMenu() = default;
    void renderMenu(float x, float y);
};

using SceneMenuPtr = std::unique_ptr<SceneMenu>;
