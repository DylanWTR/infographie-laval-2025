#pragma once

#define GL_SILENCE_DEPRECATION

// Header File Include //
#include "application/menu/configuration-menu.hpp"
#include "application/menu/primitive-menu.hpp"
#include "application/menu/model-menu.hpp"
#include "application/menu/graph-menu.hpp"
#include "application/menu/illumination-menu.hpp"
#include "application/menu/scene-menu.hpp"
#include "application/menu/bezier-surface-menu.hpp"
#include "application/menu/catmull-rom-menu.hpp"
#include "application/menu/light-menu.hpp"
#include "window/window.hpp"
#include "pipeline/pipeline.hpp"
#include "pipeline/model.hpp"
#include "pipeline/histogram.hpp"

class Application {
    Window _window{Window::WIDTH, Window::HEIGHT};
    Pipeline _pipeline;
    std::vector<PrimitivePtr> _primitives;
    std::vector<VectorialPrimitivePtr> _vectorialPrimitives;
    Logger _logger = Logger::getInstance();

    ConfigurationMenuPtr _configurationMenu;
    GraphMenuPtr _graphMenu;
    PrimitiveMenuPtr _primitiveMenu;
    TopLeftMenuPtr _topLeftMenu;
    ModelMenuPtr _modelMenu;
    IlluminationTypeMenuPtr _illuminationMenu;
    SceneMenuPtr _sceneMenu;
    BezierSurfaceMenuPtr _bezierSurfaceMenu;
    CatmullRomMenuPtr _catmullRomMenu;
    LightMenuPtr _lightMenu;

    glm::vec2 _primitiveMenuPosition = glm::vec2(-1.0f);
    glm::vec2 _modelMenuPosition = glm::vec2(-1.0f);
    glm::vec2 _sceneMenuPosition{0.0f, 300.0f};
    PrimitivePtr _selectedPrimitive = nullptr;
    bool _showImgui = true;

    void initializeDefaultScene();

public:
    Application();

    Application(const Application &) = delete;

    void initializeImgui() const;

    void run();

    void render();

    void handleMouseButton();

    void handleKey();

    void handleLeftClick(const Coordinates &coordinates);

    void handleRightClick(const Coordinates &coordinates, int mods);

    void createModel(const PrimitivePtr& primitive);

    void takeScreenshot() const;

    void deletePrimitive(const PrimitivePtr &primitive);

    Application &operator=(const Application &) = delete;

    ~Application();
};
