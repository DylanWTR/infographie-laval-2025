#pragma once

#include <vector>

#include "application/menu/floating-menu.hpp"
#include "application/primitive-factory.hpp"
#include "application/vectorial-primitive-factory.hpp"

using CreatePrimitiveFunction = std::function<void(const PrimitivePtr &)>;

class PrimitiveMenu final : public FloatingMenu {
    std::vector<PrimitiveType> _primitiveTypes = {CUBE, PLANE, CYLINDER, CONE, SPHERE};
    std::vector<VectorialPrimitiveType> _vectorialPrimitiveTypes = {POINT, LINE, SQUARE, RECTANGLE, TRIANGLE};
    unsigned _index = 0;

    CreatePrimitiveFunction _createPrimitive;

public:
    explicit PrimitiveMenu(CreatePrimitiveFunction createPrimitive);

    void up();

    void down();

    void enter();

    void renderMenu() override;
};

using PrimitiveMenuPtr = std::unique_ptr<PrimitiveMenu>;
