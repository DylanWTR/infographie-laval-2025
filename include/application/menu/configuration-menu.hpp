#pragma once

#include "pipeline/primitives/primitive.hpp"
#include "application/menu/lateral-menu.hpp"
#include "window/window.hpp"

using DeletePrimitiveFunction = std::function<void(const PrimitivePtr &primitive)>;

class ConfigurationMenu final : public UpperLateralMenu {
    Window &_window;
    PrimitivePtr _selectedPrimitive = nullptr;
    DeletePrimitiveFunction _deletePrimitive;

    static void renderFloatProperty(const FloatPropertyPtr &property);
    static void renderIntProperty(const IntPropertyPtr &property);
    static void renderBoolProperty(const BoolPropertyPtr &property);
    static void renderStringProperty(const StringPropertyPtr &property);
public:
    explicit ConfigurationMenu(Window &window, const DeletePrimitiveFunction &deletePrimitive);

    void updateSelectedPrimitive(const PrimitivePtr &selectedPrimitive);

    void renderMenu() override;
};

using ConfigurationMenuPtr = std::unique_ptr<ConfigurationMenu>;
