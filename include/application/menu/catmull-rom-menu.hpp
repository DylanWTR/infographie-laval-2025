#pragma once

#include "pipeline/primitives/catmull-rom-curve.hpp"

class CatmullRomMenu {
    CatmullRomCurve& _curve;

public:
    explicit CatmullRomMenu(CatmullRomCurve& curve);
    void render();
};

using CatmullRomMenuPtr = std::unique_ptr<CatmullRomMenu>;