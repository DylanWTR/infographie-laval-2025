#pragma once

#include <memory>
#include <string>
#include <vector>
#include "pipeline/primitives/primitive.hpp"
#include "application/light-repository.hpp"

class UpperLateralMenu {
    static constexpr int WIDTH = 300;
    std::string _title;

public:
    explicit UpperLateralMenu(std::string title);

    virtual void renderMenu() = 0;

    void render();

    virtual ~UpperLateralMenu() = default;
};

class LowerLateralMenu {
    private:
        static constexpr int WIDTH = 300;
        std::string _title;

    public:
        explicit LowerLateralMenu(std::string title);
        virtual void renderMenu(PrimitivePtr &selectedPrimitive) = 0;
        void render(PrimitivePtr &selectedPrimitive);
        virtual ~LowerLateralMenu() = default;
};

class TopLeftMenu {
    static constexpr int WIDTH = 500;
    std::string _title;

public:
    explicit TopLeftMenu(std::string title);
    virtual void renderMenu() = 0;
    void render();
    virtual ~TopLeftMenu() = default;
};

class LowerMenu {
        static constexpr int WIDTH = 300;
        std::string _title;

    public:
        explicit LowerMenu(std::string title);
        virtual void renderMenu(Shader &textureShader) = 0;
        void render();
        virtual ~LowerMenu() = default;
};

class TopMenu {
        std::string _title;
    public:
        explicit TopMenu(std::string title);
        virtual void renderMenu(std::vector<Light> &pointLights) = 0;
        void render();
        virtual ~TopMenu() = default;
};

using UpperLateralMenuPtr = std::unique_ptr<UpperLateralMenu>;
using LowerLateralMenuPtr = std::unique_ptr<LowerLateralMenu>;
using TopLeftMenuPtr = std::unique_ptr<TopLeftMenu>;
using LowerMenuPtr = std::unique_ptr<LowerMenu>;
using TopMenuPtr = std::unique_ptr<TopMenu>;
