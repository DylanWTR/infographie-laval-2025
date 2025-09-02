#pragma once

#include <string>
#include <memory>

class FloatingMenu {
    static constexpr int WIDTH = 200;
    std::string _title;

public:
    explicit FloatingMenu(std::string title);

    virtual void renderMenu() = 0;

    void render(float x, float y);

    virtual ~FloatingMenu() = default;
};

using FloatingMenuPtr = std::unique_ptr<FloatingMenu>;
