#include "application/menu/light-menu.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

LightMenu::LightMenu() : TopMenu("Light Manager") {}

void LightMenu::renderMenu(std::vector<Light> &pointLights) {
    static size_t selectedLight = -1;
    ImGui::BeginChild("Light List", ImVec2(150, 0), true);

    if (ImGui::Button("+ Add Light")) {
        pointLights.push_back({
            glm::vec3(0, 1, 0),
            glm::vec3(0),
            glm::vec3(1, 1, 1)
        });
        selectedLight = pointLights.size() - 1;
    }
    
    ImGui::Separator();
    for (size_t i = 0; i < pointLights.size(); i++) {
        char label[32];
        snprintf(label, sizeof(label), "Light %zu", i);  // Use %zu for size_t
        if (ImGui::Selectable(label, selectedLight == i)) {
            selectedLight = i;
        }
    }
    ImGui::EndChild();
    
    ImGui::SameLine();
    
    ImGui::BeginGroup();
    ImGui::BeginChild("Light Properties", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
    
    if (selectedLight != static_cast<size_t>(-1) && selectedLight < pointLights.size()) {
        Light& light = pointLights[selectedLight];
        
        // Position controls
        ImGui::Text("Position");
        ImGui::DragFloat("X", &light.position.x, 0.1f);
        ImGui::DragFloat("Y", &light.position.y, 0.1f);
        ImGui::DragFloat("Z", &light.position.z, 0.1f);
        
        // Color controls
        ImGui::Text("Color");
        ImGui::ColorEdit3("Color", &light.color.r);
        
    } else {
        ImGui::Text("Select a light or create a new one");
    }
    
    ImGui::EndChild();
    
    if (selectedLight != static_cast<size_t>(-1) && selectedLight < pointLights.size()) {
        if (ImGui::Button("Delete Selected", ImVec2(-1, 0))) {
            pointLights.erase(pointLights.begin() + selectedLight);
            selectedLight = -1;
        }
    }
    
    ImGui::EndGroup();
}