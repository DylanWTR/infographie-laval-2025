#pragma once

#include "application/menu/lateral-menu.hpp"
#include "pipeline/shader-factory.hpp"

class Application;

class IlluminationTypeMenu final : public LowerMenu {
public:
    explicit IlluminationTypeMenu();

    void renderMenu(Shader &textureShader) override;

private:
    int _currentModel = 4;
};

using IlluminationTypeMenuPtr = std::unique_ptr<IlluminationTypeMenu>;
