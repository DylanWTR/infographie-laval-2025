#pragma once

#include "application/menu/floating-menu.hpp"

class Application;

class ModelMenu final : public FloatingMenu {
    Application &_application;

    std::string _modelPath = "resources/object/";
    bool _gamma = false;


public:
    explicit ModelMenu(Application &application);

    void renderMenu() override;
};

using ModelMenuPtr = std::unique_ptr<ModelMenu>;
