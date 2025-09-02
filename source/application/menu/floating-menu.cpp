#include "application/menu/floating-menu.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

FloatingMenu::FloatingMenu(std::string title) : _title(std::move(title)) {

}

void FloatingMenu::render(const float x, const float y) {
    ImGui::SetNextWindowSize(ImVec2(WIDTH, -1));
    ImGui::SetNextWindowPos(ImVec2(x, y));

    ImGui::Begin(_title.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    renderMenu();
    ImGui::End();
}
