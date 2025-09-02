#pragma once

#include "pipeline/primitives/bezier_surface.hpp"

class BezierSurfaceMenu {
    std::shared_ptr<BezierSurface> _surface;

public:
    explicit BezierSurfaceMenu(const std::shared_ptr<BezierSurface>& surface);

    void render();
};

using BezierSurfaceMenuPtr = std::unique_ptr<BezierSurfaceMenu>;
