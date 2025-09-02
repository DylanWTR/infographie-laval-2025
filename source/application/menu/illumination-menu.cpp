#include "application/menu/illumination-menu.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stdio.h>

IlluminationTypeMenu::IlluminationTypeMenu() : LowerMenu("Illumination Type") {
}

void IlluminationTypeMenu::renderMenu(Shader &textureShader) {
    ImGui::Text("Illumination Model:");

    const char* labels[] = { "Lambert", "Gouraud", "Phong", "Blinn-Phong", "PBR" };

    for (int i = 0; i < 5; ++i) {
        bool selected = (_currentModel == i);
        if (ImGui::Checkbox(labels[i], &selected)) {
            if (selected)
                _currentModel = i;
        }

        if (selected && _currentModel != i)
            _currentModel = i;
        textureShader.setInt("illuminationModel", _currentModel);
    }
}