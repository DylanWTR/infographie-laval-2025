// Header File Include //
#include "window/window.hpp"

// ImGui Include //
#include <imgui.h>

Window::Window(const int width, const int height) : _width{width}, _height{height}, _cursor(nullptr) {
    if (!glfwInit()) {
        _logger.critical("Failed to initialize GLFW.");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    _window = glfwCreateWindow(_width, _height, "Infographie", nullptr, nullptr);
    if (_window == nullptr) {
        _logger.critical("Failed to create GLFW window.");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(_window);
    glfwSetWindowUserPointer(_window, this);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetFramebufferSizeCallback(_window, [](GLFWwindow *, const int frameWidth, const int frameHeight) {
        glViewport(0, 0, frameWidth, frameHeight);
    });
    glfwSetCursorPosCallback(_window, mouseCallback);
    glfwSetScrollCallback(_window, scrollCallback);
    glfwSetMouseButtonCallback(_window, mouseButtonCallback);
    glfwSetKeyCallback(_window, keyCallback);
    _cursor = new Cursor(_window);
}

void Window::mouseCallback(GLFWwindow *window, const double deltaX, const double deltaY) {
    if (ImGui::GetIO().WantCaptureMouse) return;
    if (auto *application = static_cast<Window *>(glfwGetWindowUserPointer(window)))
        application->handleMouseCallback(deltaX, deltaY);
}

void Window::scrollCallback(GLFWwindow *window, const double deltaX, const double deltaY) {
    if (ImGui::GetIO().WantCaptureMouse) return;
    if (auto *application = static_cast<Window *>(glfwGetWindowUserPointer(window)))
        application->handleScrollCallback(deltaX, deltaY);
}

void Window::mouseButtonCallback(GLFWwindow *window, const int button, const int action, const int mods) {
    if (ImGui::GetIO().WantCaptureMouse) return;
    if (const auto *application = static_cast<Window *>(glfwGetWindowUserPointer(window)))
        application->handleMouseButtonCallback(button, action, mods);
}

void Window::keyCallback(GLFWwindow *window, const int key, int scancode, const int action, const int mods) {
    if (const auto *application = static_cast<Window *>(glfwGetWindowUserPointer(window)))
        application->handleKeyCallback(key, scancode, action, mods);
}

void Window::processInput() {
    const float cameraSpeed = 5 * _deltaTime;

    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(_window, true);

    if (glfwGetKey(_window, GLFW_KEY_F1) == GLFW_PRESS)
        _projectionMode = PERSPECTIVE;
    if (glfwGetKey(_window, GLFW_KEY_F2) == GLFW_PRESS)
        _projectionMode = ORTHOGRAPHIC;

    if (ImGui::GetIO().WantCaptureMouse) return;
    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
        _cameraPosition += cameraSpeed * _cameraMouseDirection;
    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
        _cameraPosition += -(cameraSpeed * _cameraMouseDirection);
    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
        _cameraPosition += -normalize(cross(_cameraMouseDirection, _cameraY)) * cameraSpeed;
    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
        _cameraPosition += normalize(cross(_cameraMouseDirection, _cameraY)) * cameraSpeed;
}

void Window::handleMouseCallback(const double deltaX, const double deltaY) {
    constexpr float sensitivity = 0.1f;
    const auto xPosition = static_cast<float>(deltaX);
    const auto yPosition = static_cast<float>(deltaY);

    if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE) {
        _initial = true;
        setCursor(CursorType::ARROW);
    }

    if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_1) != GLFW_PRESS)
        return;
    if (_initial) {
        _lastX = xPosition;
        _lastY = yPosition;
        _initial = false;
    }

    const float xOffset = (xPosition - _lastX) * sensitivity;
    const float yOffset = (_lastY - yPosition) * sensitivity;

    _lastX = xPosition;
    _lastY = yPosition;

    _yaw += xOffset;
    _pitch += yOffset;

    if (_pitch > 89.0f)
        _pitch = 89.0f;
    if (_pitch < -89.0f)
        _pitch = -89.0f;
    _cameraMouseDirection = normalize(glm::vec3(
        cos(glm::radians(_yaw)) * cos(glm::radians(_pitch)),
        sin(glm::radians(_pitch)),
        sin(glm::radians(_yaw)) * cos(glm::radians(_pitch))
    ));

    setCursor(CursorType::RESIZE_ALL);
}

void Window::handleScrollCallback(const double, const double deltaY) {
    _fov -= static_cast<float>(deltaY);

    if (_fov < 1.0f)
        _fov = 1.0f;
    if (_fov > 90.0f)
        _fov = 90.0f;

    setCursor(CursorType::RESIZE_VERTICAL);
}

void Window::handleMouseButtonCallback(const MouseReleaseCallback &callback) {
    _mouseReleaseCallback = callback;
}

void Window::handleKeyCallback(const int key, const int scancode, const int action, const int mods) const {
    if (action != GLFW_RELEASE) return;

    try {
        _keyCallback(key, scancode, action, mods);
    } catch (const std::bad_function_call &) {
        _logger.warn("Unable to callback key listener");
    }
}

void Window::handleKeyCallback(const KeyCallback &callback) {
    _keyCallback = callback;
}

void Window::handleMouseButtonCallback(const int button, const int action, const int mods) const {
    if (action != GLFW_RELEASE) return;

    try {
        _mouseReleaseCallback(button, mods);
    } catch (const std::bad_function_call &) {
        _logger.warn("Unable to callback mouse listener");
    }
}

GLFWwindow *Window::getWindowInstance() const {
    return _window;
}

int Window::getWidth() const {
    return _width;
}

int Window::getHeight() const {
    return _height;
}

float Window::getLastFrame() const {
    return _lastFrame;
}

glm::vec3 Window::getCameraPosition() const {
    return _cameraPosition;
}

bool Window::isMovingCamera() const {
    return !_initial;
}

float Window::getAspectRatio() const {
    return static_cast<float>(_width) / static_cast<float>(_height);
}

glm::mat4 Window::getProjection() const {
    const float aspectRatio = getAspectRatio();
    const float orthoWidth = static_cast<float>(_width) / 200.0f;
    const float orthoWHeight = static_cast<float>(_height) / 200.0f;
    constexpr float zNear = 0.1f;
    constexpr float zFar = 100.0f;

    if (_projectionMode == PERSPECTIVE)
        return glm::perspective(glm::radians(_fov), aspectRatio, zNear, zFar);
    return glm::ortho(-orthoWidth, orthoWidth, -orthoWHeight, orthoWHeight, zNear, zFar);
}

glm::mat4 Window::getView() const {
    return lookAt(_cameraPosition, _cameraPosition + _cameraMouseDirection, _cameraY);
}

ProjectionMode Window::getProjectionMode() const {
    return _projectionMode;
}

void Window::setCameraPosition(const glm::vec3 newPosition) {
    _cameraPosition += newPosition;
}

void Window::setDeltaTime(const float newDeltaTime) {
    _deltaTime = newDeltaTime;
}

void Window::setLastFrame(const float newLastFrame) {
    _lastFrame = newLastFrame;
}

Window::~Window() {
    delete _cursor;
    glfwDestroyWindow(_window);
    glfwTerminate();
}
