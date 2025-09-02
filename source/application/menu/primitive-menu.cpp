#include <imgui.h>

#include <utility>

#include "application/menu/primitive-menu.hpp"

PrimitiveMenu::PrimitiveMenu(CreatePrimitiveFunction createPrimitive) : FloatingMenu("New Primitive"),
                                                                        _createPrimitive(std::move(createPrimitive)) {
}

void PrimitiveMenu::up() {
    if (_index == 0)
        _index = static_cast<unsigned>(_primitiveTypes.size() + _vectorialPrimitiveTypes.size());
    _index--;
}

void PrimitiveMenu::down() {
    _index++;
    if (_index >= _primitiveTypes.size() + _vectorialPrimitiveTypes.size())
        _index = 0;
}

void PrimitiveMenu::enter() {
    PrimitivePtr primitive;
    if (_index < _primitiveTypes.size())
        primitive = PrimitiveFactory::createPrimitive(_primitiveTypes[_index]);
    else primitive = VectorialPrimitiveFactory::createPrimitive(
             _vectorialPrimitiveTypes[_index - _primitiveTypes.size()]);

    _index = 0;
    _createPrimitive(primitive);
}

void PrimitiveMenu::renderMenu() {
    constexpr auto redColor = ImVec4(1.0f, 0.5f, 0.5f, 1.0f);
    constexpr auto whiteColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    for (unsigned index = 0; index < _primitiveTypes.size() + _vectorialPrimitiveTypes.size(); index++) {
        const std::string name = "> " + (index < _primitiveTypes.size()
                                             ? primitiveTypeNames[index]
                                             : vectorialPrimitiveTypeNames[index - _primitiveTypes.size()]);

        if (index != 0) {
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
        }
        ImGui::TextColored(_index == index ? redColor : whiteColor, "%s", name.c_str());
    }

    ImGui::Dummy(ImVec2(0.0f, 10.0f));
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Use <up/down> and <enter>");
}
