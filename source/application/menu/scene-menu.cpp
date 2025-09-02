#include <imgui.h>

#include "application/menu/scene-menu.hpp"

void SceneMenu::renderMenu(float x, float y) {
    auto &[enableToneMapping, toneMappingExposure] = ToneMappingSettings::instance();

    ImGui::SetNextWindowPos({ x, y }, ImGuiCond_Once);
    ImGui::Begin("Scene Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);

    ImGui::Checkbox("Enable Tone Mapping", &enableToneMapping);
    if (enableToneMapping) {
        ImGui::SliderFloat("Exposure", &toneMappingExposure, 0.1f, 5.0f);
    }

    ImGui::End();
}
