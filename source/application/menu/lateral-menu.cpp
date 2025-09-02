#include "application/menu/lateral-menu.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "pipeline/shader-factory.hpp"

#include <utility>

UpperLateralMenu::UpperLateralMenu(std::string title): _title(std::move(title)) {}

LowerLateralMenu::LowerLateralMenu(std::string title) : _title(std::move(title)) {}

TopLeftMenu::TopLeftMenu(std::string title) : _title(std::move(title)) {}

LowerMenu::LowerMenu(std::string title) : _title(std::move(title)) {}

TopMenu::TopMenu(std::string title) : _title(std::move(title)) {}

void UpperLateralMenu::render() {
    const ImGuiIO &io = ImGui::GetIO();

    ImGui::SetNextWindowSize(ImVec2(WIDTH, io.DisplaySize.y / 2));
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - WIDTH, 0));

    ImGui::Begin(_title.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    renderMenu();
    ImGui::End();
}

void LowerLateralMenu::render(PrimitivePtr &selectedPrimitive) {
    const ImGuiIO &io = ImGui::GetIO();

    ImGui::SetNextWindowSize(ImVec2(WIDTH, io.DisplaySize.y / 4));
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - WIDTH, io.DisplaySize.y / 2));
    ImGui::Begin(_title.c_str(), nullptr, ImGuiWindowFlags_NoCollapse);    
    renderMenu(selectedPrimitive);
    ImGui::End();
}

void TopLeftMenu::render() {
    const ImGuiIO &io = ImGui::GetIO();

    ImGui::SetNextWindowSize(ImVec2(WIDTH, io.DisplaySize.y / 3.8));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin(_title.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    renderMenu();
    ImGui::End();
}

void LowerMenu::render() {
    const ImGuiIO &io = ImGui::GetIO();

    ImGui::SetNextWindowSize(ImVec2(WIDTH, io.DisplaySize.y / 4));
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - WIDTH, (io.DisplaySize.y / 2 + io.DisplaySize.y / 4)));
    ImGui::Begin(_title.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    renderMenu(ShaderFactory::getInstance().getTextureShader());
    ImGui::End();
}

void TopMenu::render() {
    const ImGuiIO &io = ImGui::GetIO();

    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x / 3, io.DisplaySize.y / 4));
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 3, 0));
    ImGui::Begin(_title.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    renderMenu(LightRepository::getInstance().getPointLights());
    ImGui::End();
}
