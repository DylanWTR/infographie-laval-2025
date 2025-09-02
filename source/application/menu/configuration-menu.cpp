#include <imgui.h>
#include <imgui_stdlib.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "application/menu/configuration-menu.hpp"


ConfigurationMenu::ConfigurationMenu(Window &window, const DeletePrimitiveFunction &deletePrimitive) : UpperLateralMenu("Configuration"), _window(window), _deletePrimitive(deletePrimitive) {}

void ConfigurationMenu::updateSelectedPrimitive(const PrimitivePtr &selectedPrimitive) {
    _selectedPrimitive = selectedPrimitive;
}

void ConfigurationMenu::renderFloatProperty(const FloatPropertyPtr &property) {
    const auto id = std::format("##{}{}", std::to_string(property->type()), property->name());
    float value = property->value();

    ImGui::Text("%s", property->name().data());
    ImGui::InputFloat(id.c_str(), &value);
    if (value != property->value() && !std::isnan(value))
        property->updateValue(value);
}

void ConfigurationMenu::renderIntProperty(const IntPropertyPtr &property) {
    const auto id = std::format("##{}{}", std::to_string(property->type()), property->name());
    int value = property->value();

    if (property->type() == FILTER) {
        static const char *FILTER_NAMES[] = { "None", "Grayscale", "Inverted", "Posterize"};
        static constexpr int FILTER_COUNT = std::size(FILTER_NAMES);

        ImGui::Text("%s", property->name().data());
        if (ImGui::Combo(id.c_str(), &value, FILTER_NAMES, FILTER_COUNT)) {
            value = std::clamp(value, 0, FILTER_COUNT - 1);
            property->updateValue(value);
        }
    } else {
        ImGui::Text("%s", property->name().data());
        ImGui::InputInt(id.c_str(), &value);
        if (value != property->value() && !std::isnan(value))
            property->updateValue(value);
    }
}

void ConfigurationMenu::renderBoolProperty(const BoolPropertyPtr &property) {
    const auto id = std::format("##{}{}", std::to_string(property->type()), property->name());
    bool value = property->value();

    ImGui::Text("%s", property->name().data());
    ImGui::Checkbox(id.c_str(), &value);
    if (value != property->value())
        property->updateValue(value);
}

void ConfigurationMenu::renderStringProperty(const StringPropertyPtr &property) {
    const auto id = std::format("##{}{}", std::to_string(property->type()), property->name());
    std::string buffer = property->value();

    ImGui::Text("%s", property->name().data());
    if (ImGui::InputText(id.c_str(), &buffer, ImGuiInputTextFlags_EnterReturnsTrue)) {
        if (const std::string bufferString(buffer); bufferString != property->value())
            property->updateValue(bufferString);
    }
}

void ConfigurationMenu::renderMenu() {
    bool first = true;

    if (_selectedPrimitive == nullptr) {
        ImGui::Text("No object selected in the scene!");
        return;
    }
    for (const auto &property: _selectedPrimitive->getProperties()) {
        const auto floatProperty = std::dynamic_pointer_cast<FloatProperty>(property);
        const auto intProperty = std::dynamic_pointer_cast<IntProperty>(property);
        const auto boolProperty = std::dynamic_pointer_cast<BoolProperty>(property);
        const auto stringProperty = std::dynamic_pointer_cast<StringProperty>(property);

        if (property->type() == CATEGORY) {
            ImGui::Spacing();
            if (!first)
                ImGui::Separator();
            ImGui::Text("> %s", property->name().data());
            ImGui::Spacing();
            first = false;
            continue;
        }
        if (floatProperty != nullptr)
            renderFloatProperty(floatProperty);
        if (intProperty != nullptr)
            renderIntProperty(intProperty);
        if (boolProperty != nullptr)
            renderBoolProperty(boolProperty);
        if (stringProperty != nullptr)
            renderStringProperty(stringProperty);
    }

    ImGui::Separator();

    if (ImGui::Button("Delete")) {
        _deletePrimitive(_selectedPrimitive);
        _selectedPrimitive = nullptr;
    }

    if (ImGui::IsItemHovered()) {
        _window.setCursor(CursorType::HAND);
    } else {
        _window.setCursor(CursorType::ARROW);
    }
}
