// Header File Include //
#include "application/application.hpp"
#include "pipeline/texture-loader.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// GLM Include //
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ImGui Include //
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// STD Include //
#include <filesystem>
#include <chrono>
#include <vector>

void Application::initializeDefaultScene() {
    const auto backpack = std::make_shared<Model>("resources/object/backpack/backpack.obj");

    const auto cube = PrimitiveFactory::createPrimitive(CUBE);
    const auto sphere = PrimitiveFactory::createPrimitive(SPHERE);
    const auto cylinder = PrimitiveFactory::createPrimitive(CYLINDER);
    const auto cone = PrimitiveFactory::createPrimitive(CONE);
    const auto ground = PrimitiveFactory::createPrimitive(PLANE);

    const auto pointLightBoxA = PrimitiveFactory::createPrimitive(CUBE);
    const auto pointLightBoxB = PrimitiveFactory::createPrimitive(CUBE);
    const auto pointLight = PrimitiveFactory::createPrimitive(SPHERE);

    auto bezierSurface = std::dynamic_pointer_cast<BezierSurface>(PrimitiveFactory::createPrimitive(BEZIER_SURFACE));
    const auto catmullRomCurve = std::dynamic_pointer_cast<CatmullRomCurve>(PrimitiveFactory::createPrimitive(CATMULL_ROM_CURVE));

    const auto mattePlasticSphere = PrimitiveFactory::createPrimitive(SPHERE);
    const auto whiteChromeSphere = PrimitiveFactory::createPrimitive(SPHERE);
    const auto roughGoldSphere = PrimitiveFactory::createPrimitive(SPHERE);
    const auto glowyCeramicSphere  = PrimitiveFactory::createPrimitive(SPHERE);
    const auto saturatedMetalSphere  = PrimitiveFactory::createPrimitive(SPHERE);

    backpack->translate(glm::vec3(0, -3, 0));
    backpack->scale(0.5);
    backpack->setRoughness(0);
    backpack->setMetallic(1);

    cube->translate(glm::vec3(-6, -3, 0));
    cube->loadTexture("resources/textures/container.jpg");

    sphere->translate(glm::vec3(-3, -3, 0));
    sphere->loadTexture("resources/textures/container.jpg");

    cylinder->translate(glm::vec3(3, -3, 0));
    cylinder->loadTexture("resources/textures/container.jpg");

    cone->translate(glm::vec3(6, -3, 0));
    cone->loadTexture("resources/textures/container.jpg");

    ground->translate(glm::vec3(0, -5, 0));
    ground->scale(20);
    ground->setColor(glm::vec3(1, 1, 1));
    ground->setRoughness(1);
    ground->setMetallic(0);

    pointLightBoxA->translate(glm::vec3(10, 0, -1.5));
    pointLightBoxA->loadTexture("resources/textures/container.jpg");

    pointLightBoxB->translate(glm::vec3(15, 0, -1.5));
    pointLightBoxB->loadTexture("resources/textures/container.jpg");

    // Sphere light //
    pointLight->translate(glm::vec3(11.5f, 1.5f, 0.0f));
    pointLight->setColor(glm::vec3(1.0f, 1.0f, 0.0f));
    pointLight->setMetallic(1);
    pointLight->setRoughness(0);
    pointLight->scale(0.1f);

    bezierSurface->translate(glm::vec3(-10, -3, 0));
    bezierSurface->scale(glm::vec3(-1.0f, -1.0f, -1.0f));
    bezierSurface->setColor(glm::vec3(1, 0, 1));
    bezierSurface->setRoughness(0.99);
    bezierSurface->setMetallic(0);
    _bezierSurfaceMenu = std::make_unique<BezierSurfaceMenu>(bezierSurface);

    catmullRomCurve->setControlPoints({
        { -5, 0, 0 },
        { -3, 1, 0 },
        {  0, -1, 0 },
        {  3, 2, 0 },
        {  5, 0, 0 },
        {  7, -1, 0 }
    });
    catmullRomCurve->translate(glm::vec3(0, 4, 0));
    catmullRomCurve->setColor(glm::vec3(0, 1, 1));
    catmullRomCurve->setRoughness(0.1);
    catmullRomCurve->setMetallic(0.2);
    _catmullRomMenu = std::make_unique<CatmullRomMenu>(*catmullRomCurve);

    whiteChromeSphere->translate(glm::vec3(-6, 1, 0));
    whiteChromeSphere->setColor(glm::vec3(1, 1, 1));
    whiteChromeSphere->setRoughness(0);
    whiteChromeSphere->setMetallic(1);

    roughGoldSphere->translate(glm::vec3(-3, 1, 0));
    roughGoldSphere->setColor(glm::vec3(1.0, 0.71, 0.29));
    roughGoldSphere->setRoughness(0.5);
    roughGoldSphere->setMetallic(1);

    mattePlasticSphere->translate(glm::vec3(0, 1, 0));
    mattePlasticSphere->setColor(glm::vec3(0.8, 0.1, 0.1));
    mattePlasticSphere->setRoughness(0.99);
    mattePlasticSphere->setMetallic(0);

    glowyCeramicSphere->translate(glm::vec3(3, 1, 0));
    glowyCeramicSphere->setColor(glm::vec3(1, 1, 1));
    glowyCeramicSphere->setRoughness(0.1);
    glowyCeramicSphere->setMetallic(0);

    saturatedMetalSphere->translate(glm::vec3(6, 1, 0));
    saturatedMetalSphere->setColor(glm::vec3(0.0, 0.8, 1.0));
    saturatedMetalSphere->setRoughness(0.1);
    saturatedMetalSphere->setMetallic(0);

    _primitives.push_back(backpack);

    _primitives.push_back(cube);
    _primitives.push_back(sphere);
    _primitives.push_back(cylinder);
    _primitives.push_back(cone);
    _primitives.push_back(ground);

    _primitives.push_back(mattePlasticSphere);
    _primitives.push_back(whiteChromeSphere);
    _primitives.push_back(roughGoldSphere);
    _primitives.push_back(glowyCeramicSphere);
    _primitives.push_back(saturatedMetalSphere);

    _primitives.push_back(pointLightBoxA);
    _primitives.push_back(pointLightBoxB);
    _primitives.push_back(pointLight);

    _primitives.push_back(bezierSurface);
    _primitives.push_back(catmullRomCurve);
}

Application::Application() {
    _primitiveMenu = std::make_unique<PrimitiveMenu>([this](const PrimitivePtr &primitive) {
        _primitives.emplace_back(primitive);
    });

    _topLeftMenu = std::make_unique<Histogram>(_window);
    _modelMenu = std::make_unique<ModelMenu>(*this);
    _graphMenu = std::make_unique<GraphMenu>(_window, _primitives);
    _configurationMenu = std::make_unique<ConfigurationMenu>(_window, [this](const PrimitivePtr &primitive) {
        _logger.trace("Delete primitive from scene");
        deletePrimitive(primitive);
    });
    _illuminationMenu = std::make_unique<IlluminationTypeMenu>();
    _sceneMenu = std::make_unique<SceneMenu>();
    _lightMenu = std::make_unique<LightMenu>();

    initializeDefaultScene();
    initializeImgui();
}

void Application::initializeImgui() const {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    ImGui_ImplGlfw_InitForOpenGL(_window.getWindowInstance(), true);
    ImGui_ImplOpenGL3_Init();

    glfwSetMouseButtonCallback(_window.getWindowInstance(), [](GLFWwindow *window, int button, int action, int mods) {
        ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    });
    glfwSetScrollCallback(_window.getWindowInstance(), [](GLFWwindow *window, double xoffset, double yoffset) {
        ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    });
    glfwSetKeyCallback(_window.getWindowInstance(),
                       [](GLFWwindow *window, int key, int scancode, int action, int mods) {
                           ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
                       });
    glfwSetCharCallback(_window.getWindowInstance(), [](GLFWwindow *window, unsigned int c) {
        ImGui_ImplGlfw_CharCallback(window, c);
    });
}

void Application::run() {
    GLFWwindow *window = _window.getWindowInstance();

    while (!glfwWindowShouldClose(window)) {
        const auto currentFrame = static_cast<float>(glfwGetTime());

        _window.setDeltaTime(currentFrame - _window.getLastFrame());
        _window.setLastFrame(currentFrame);

        glfwPollEvents();
        _window.processInput();
        handleMouseButton();
        handleKey();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
}

void Application::render() {
    const glm::mat4 view = _window.getView();
    const glm::mat4 projection = _window.getProjection();

    if (_showImgui) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    _pipeline.render(_primitives, view, projection);
    if (_selectedPrimitive != nullptr)
        _selectedPrimitive->renderSelectionBox(view, projection);
    if (_primitiveMenuPosition.x != -1.0f && _primitiveMenuPosition.y != -1.0f)
        _primitiveMenu->render(_primitiveMenuPosition.x, _primitiveMenuPosition.y);
    if (_modelMenuPosition.x != -1.0f && _modelMenuPosition.y != -1.0f)
        _modelMenu->render(_modelMenuPosition.x, _modelMenuPosition.y);
    if (_showImgui) {
        _lightMenu->render();
        _illuminationMenu->render();
        _configurationMenu->updateSelectedPrimitive(_selectedPrimitive);
        _configurationMenu->render();
        _graphMenu->render(_selectedPrimitive);
        _topLeftMenu->render();
        _sceneMenu->renderMenu(_sceneMenuPosition.x, _sceneMenuPosition.y);
        _bezierSurfaceMenu->render();
        _catmullRomMenu->render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    if (!_showImgui) {
        takeScreenshot();
        _showImgui = true;
    }
}

void Application::handleMouseButton() {
    _window.handleMouseButtonCallback([this](const int button, const int mods) {
        double mouseX;
        double mouseY;

        if (_window.isMovingCamera() || ImGui::GetIO().WantCaptureMouse) return;
        glfwGetCursorPos(_window.getWindowInstance(), &mouseX, &mouseY);
        if (button == GLFW_MOUSE_BUTTON_LEFT)
            handleLeftClick(std::make_pair(mouseX, mouseY));
        if (button == GLFW_MOUSE_BUTTON_RIGHT)
            handleRightClick(std::make_pair(mouseX, mouseY), mods);
    });
}

void Application::handleKey() {
    _window.handleKeyCallback([this](const int key, int, const int action, const int mods) {
        if (action != GLFW_RELEASE) return;

        if (key == GLFW_KEY_I && (mods & GLFW_MOD_CONTROL)) {
            _showImgui = !_showImgui;
            _logger.info("Visibility toggled: {}", _showImgui ? "on" : "off");
        }

        if (_primitiveMenuPosition.x != -1.0f && _primitiveMenuPosition.y != -1.0f) {
            if (key == GLFW_KEY_UP)
                _primitiveMenu->up();
            if (key == GLFW_KEY_DOWN)
                _primitiveMenu->down();
            if (key == GLFW_KEY_ENTER) {
                _primitiveMenu->enter();
                _primitiveMenuPosition = glm::vec2(-1.0f);
            }
        }
    });
}

void Application::takeScreenshot() const {
    const auto now = std::chrono::system_clock::now();
    const auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    const std::string filename = "screenshots/screenshot_" + std::to_string(timestamp) + ".png";
    int width;
    int height;

    glfwGetFramebufferSize(_window.getWindowInstance(), &width, &height);
    std::vector<unsigned char> pixels(width * height * 3);
    std::vector<unsigned char> flippedPixels(width * height * 3);

    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
    for (int y = 0; y < height; y++)
        memcpy(&flippedPixels[y * width * 3], &pixels[(height - 1 - y) * width * 3], width * 3);
    std::filesystem::create_directory("screenshots");
    stbi_write_png(filename.c_str(), width, height, 3, flippedPixels.data(), width * 3);
    _logger.info("Screenshot saved to: {}", filename);
}

void Application::handleLeftClick(const Coordinates &coordinates) {
    if (!ImGui::GetIO().WantCaptureMouse) {
        _primitiveMenuPosition = glm::vec2(-1.0f);
        _modelMenuPosition = glm::vec2(-1.0f);
    }
    _selectedPrimitive = Intersection::findNearestPrimitive(_primitives, _window, coordinates);
}

void Application::handleRightClick(const Coordinates &coordinates, const int mods) {
    if (mods == GLFW_MOD_SHIFT)
        _modelMenuPosition = glm::vec2(coordinates.first, coordinates.second);
    else _primitiveMenuPosition = glm::vec2(coordinates.first, coordinates.second);
}

void Application::createModel(const PrimitivePtr &primitive) {
    _modelMenuPosition = glm::vec3(-1.0f);
    _primitives.emplace_back(primitive);
}

void Application::deletePrimitive(const PrimitivePtr &primitive) {
    _selectedPrimitive = nullptr;
    _configurationMenu->updateSelectedPrimitive(nullptr);
    if (primitive == nullptr) return;
    _primitives.erase(std::ranges::find(_primitives, primitive));
}

Application::~Application() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
