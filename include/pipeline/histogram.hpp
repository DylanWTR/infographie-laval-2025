#pragma once

#include <vector>
#include <string>
#include <array>
#include <imgui.h>

#include "application/menu/lateral-menu.hpp"
#include "window/window.hpp"

class Histogram : public TopLeftMenu {
    Window &_window;
    std::string _imagePath;
    std::array<int, 256> _grayscaleHistogram{};
    char _inputBuffer[512] = "";

public:
    explicit Histogram(Window &window, const std::string &imagePath = "resources/textures/sample.png");
    void renderMenu() override;
    void compute();
};

using HistogramPtr = std::unique_ptr<Histogram>;