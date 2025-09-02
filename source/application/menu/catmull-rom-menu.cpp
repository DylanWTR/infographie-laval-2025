#include "application/menu/catmull-rom-menu.hpp"
#include <imgui.h>

CatmullRomMenu::CatmullRomMenu(CatmullRomCurve& curve) : _curve(curve) {}

void CatmullRomMenu::render() {
    ImGui::SetNextWindowPos(ImVec2(0, 480), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(300, 55), ImGuiCond_Always);
    ImGui::Begin("Catmull-Rom Curve", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    static int resolution = 20;
    if (ImGui::SliderInt("Resolution", &resolution, 4, 100)) {
        _curve.setResolution(resolution);
    }

    ImGui::End();
}
