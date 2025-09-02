#include "pipeline/histogram.hpp"

#include <iostream>
#include <stb_image.hpp>
#include <algorithm>
#include <numeric>

Histogram::Histogram(Window &window, const std::string &imagePath)
    : TopLeftMenu("Greyscale Histogram"), _window(window), _imagePath(imagePath) {
    std::strncpy(_inputBuffer, _imagePath.c_str(), sizeof(_inputBuffer) - 1);
    _inputBuffer[sizeof(_inputBuffer) - 1] = '\0';
    compute();
}


void Histogram::compute() {
    if (_imagePath.empty()) return;

    int width, height, channels;
    unsigned char *data = stbi_load(_imagePath.c_str(), &width, &height, &channels, 0);

    if (!data) {
        std::cerr << "Impossible de charger l'image " << _imagePath << std::endl;
        return;
    }

    std::fill(_grayscaleHistogram.begin(), _grayscaleHistogram.end(), 0);

    for (int i = 0; i < width * height; ++i) {
        int r = data[i * channels];
        int g = data[i * channels + 1];
        int b = data[i * channels + 2];

        int gray = static_cast<int>(0.3 * r + 0.59 * g + 0.11 * b);
        gray = std::clamp(gray, 0, 255);

        _grayscaleHistogram[gray]++;
    }

    stbi_image_free(data);
}

void Histogram::renderMenu() {
    bool isHistogramEmpty = std::all_of(_grayscaleHistogram.begin(), _grayscaleHistogram.end(),
                                        [](int v) { return v == 0; });

    if (!isHistogramEmpty) {
        static float histogramData[256];
        int maxPixels = *std::max_element(_grayscaleHistogram.begin(), _grayscaleHistogram.end());
        if (maxPixels > 0) {
            for (int i = 0; i < 256; ++i) {
                histogramData[i] = static_cast<float>(_grayscaleHistogram[i]);
            }

            ImGui::PlotHistogram("Occurrences", histogramData, 256, 0, nullptr, 0.0f, static_cast<float>(maxPixels),
                                 ImVec2(400, 150));

            if (ImGui::IsItemHovered())
                _window.setCursor(CursorType::CROSSHAIR);

            ImGui::Text("Grayscale Intensity (0 - Black, 255 - White)");
        }
    } else {
        ImGui::Text("No histogram to display.");
    }

    ImGui::Separator();
    ImGui::Text("Enter the image path:");

    if (ImGui::InputText("##ImagePath", _inputBuffer, IM_ARRAYSIZE(_inputBuffer),
                         ImGuiInputTextFlags_EnterReturnsTrue)) {
        _imagePath = std::string(_inputBuffer);
        compute();
    }

    if (ImGui::IsItemHovered()) {
        _window.setCursor(CursorType::IBEAM);
    }

    if (ImGui::Button("Charger l'image")) {
        _imagePath = std::string(_inputBuffer);
        compute();
    }

    if (ImGui::IsItemHovered()) {
        _window.setCursor(CursorType::HAND);
    }
}
