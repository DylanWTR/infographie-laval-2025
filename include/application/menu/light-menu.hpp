#pragma once

#include "application/menu/lateral-menu.hpp"
#include "pipeline/shader-factory.hpp"
#include <vector>
#include <imgui.h>

class LightMenu final : public TopMenu {
public:
    explicit LightMenu();
    void renderMenu(std::vector<Light> &pointLights) override;
};

using LightMenuPtr = std::unique_ptr<LightMenu>;