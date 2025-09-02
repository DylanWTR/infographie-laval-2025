#include "application/menu/bezier-surface-menu.hpp"
#include <imgui.h>

BezierSurfaceMenu::BezierSurfaceMenu(const std::shared_ptr<BezierSurface>& surface)
    : _surface(surface) {}

void BezierSurfaceMenu::render() {
    ImGui::SetNextWindowPos(ImVec2(0, 400), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(300, 80), ImGuiCond_Always);
    ImGui::Begin("Bezier Surface",
                nullptr,
                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    static int resolutionU = 20;
    static int resolutionV = 20;
    if (ImGui::SliderInt("Resolution U", &resolutionU, 2, 100) ||
        ImGui::SliderInt("Resolution V", &resolutionV, 2, 100)) {
        _surface->setResolution(resolutionU, resolutionV);
    }

    ImGui::End();
}
