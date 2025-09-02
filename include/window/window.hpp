#pragma once

//GLFW Include//
#include <GLFW/glfw3.h>

// GLM Include //
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "application/logger.hpp"
#include "window/cursor.hpp"

using MouseReleaseCallback = std::function<void(int, int)>;
using KeyCallback = std::function<void(int, int, int, int)>;

enum ProjectionMode {
    PERSPECTIVE, ORTHOGRAPHIC
};

class Window {
    GLFWwindow *_window;
    int _width;
    int _height;

    // Camera Movement //
    glm::vec3 _cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 _cameraMouseDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 _cameraY = glm::vec3(0.0f, 1.0f, 0.0f);

    Cursor *_cursor;

    // Mouse Movement //
    bool _initial = true;
    float _yaw = -90.0f;
    float _pitch = 0.0f;
    float _lastX = WIDTH / 2.0;
    float _lastY = HEIGHT / 2.0;
    float _fov = 60.0f;

    // Timing //
    float _deltaTime = 0.0f;
    float _lastFrame = 0.0f;

    MouseReleaseCallback _mouseReleaseCallback;
    KeyCallback _keyCallback;

    ProjectionMode _projectionMode = PERSPECTIVE;

    Logger _logger = Logger::getInstance();

public:
    static constexpr int WIDTH = 1920;
    static constexpr int HEIGHT = 1080;

    Window(int width, int height);

    Window(const Window &) = delete;

    void processInput();

    void handleMouseCallback(double deltaX, double deltaY);

    void handleScrollCallback(double deltaX, double deltaY);

    void handleMouseButtonCallback(int button, int action, int mods) const;

    void handleMouseButtonCallback(const MouseReleaseCallback &callback);

    void handleKeyCallback(int key, int scancode, int action, int mods) const;

    void handleKeyCallback(const KeyCallback &callback);

    void setCursor(CursorType type) { _cursor->setCursor(type); }

    static void mouseCallback(GLFWwindow *window, double deltaX, double deltaY);

    static void scrollCallback(GLFWwindow *window, double deltaX, double deltaY);

    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

    [[nodiscard]] GLFWwindow *getWindowInstance() const;

    [[nodiscard]] int getWidth() const;

    [[nodiscard]] int getHeight() const;

    [[nodiscard]] float getLastFrame() const;

    [[nodiscard]] glm::vec3 getCameraPosition() const;

    [[nodiscard]] bool isMovingCamera() const;

    [[nodiscard]] float getAspectRatio() const;

    [[nodiscard]] glm::mat4 getProjection() const;

    [[nodiscard]] glm::mat4 getView() const;

    [[nodiscard]] ProjectionMode getProjectionMode() const;

    void setCameraPosition(glm::vec3 newPosition);

    void setDeltaTime(float newDeltaTime);

    void setLastFrame(float newLastFrame);

    Window &operator=(const Window &) = delete;

    ~Window();
};
