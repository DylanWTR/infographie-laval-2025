#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>

enum class CursorType {
    ARROW,
    IBEAM,
    CROSSHAIR,
    HAND,
    RESIZE_HORIZONTAL,
    RESIZE_VERTICAL,
    RESIZE_NWSE,
    RESIZE_NESW,
    RESIZE_ALL,
    NOT_ALLOWED
};

class Cursor {
    GLFWwindow *_window;
    std::unordered_map<CursorType, GLFWcursor *> _cursors;

public:
    explicit Cursor(GLFWwindow *window);

    Cursor(const Cursor &) = delete;
    Cursor &operator=(const Cursor &) = delete;

    void setCursor(CursorType type);

    ~Cursor();
};
