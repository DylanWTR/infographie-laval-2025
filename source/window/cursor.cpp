#include "window/cursor.hpp"

Cursor::Cursor(GLFWwindow *window) : _window(window) {
    _cursors[CursorType::ARROW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    _cursors[CursorType::IBEAM] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    _cursors[CursorType::CROSSHAIR] = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    _cursors[CursorType::HAND] = glfwCreateStandardCursor(GLFW_POINTING_HAND_CURSOR);
    _cursors[CursorType::RESIZE_HORIZONTAL] = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
    _cursors[CursorType::RESIZE_VERTICAL] = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
    _cursors[CursorType::RESIZE_NWSE] = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
    _cursors[CursorType::RESIZE_NESW] = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
    _cursors[CursorType::RESIZE_ALL] = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
    _cursors[CursorType::NOT_ALLOWED] = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
}

void Cursor::setCursor(CursorType type) {
    if (_cursors.find(type) != _cursors.end()) {
        glfwSetCursor(_window, _cursors[type]);
        glfwPostEmptyEvent();  // For MacOS
    }
}

Cursor::~Cursor() {
    for (auto &[type, cursor] : _cursors) {
        glfwDestroyCursor(cursor);
    }
}