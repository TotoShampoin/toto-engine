#pragma once
#include <toto-engine/import-gl.hpp>

namespace toto {

class Window {
public:
    Window(int width, int height, const char* title, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);
    ~Window();

    static void setHint(int hint, int value);

    static void initGL();

    void makeContextCurrent();

    static void pollEvents();
    void swapBuffers();
    bool shouldClose();

    GLFWwindow* handle();

private:
    GLFWwindow* _handle;
};

/**
 * GLFW initializer. Initializes upon creating the first Window, terminates when exiting the program.
 */
class GLFW {
public:
    GLFW(const GLFW&) = delete;
    GLFW& operator=(const GLFW&) = delete;
    GLFW(GLFW&&) = delete;
    GLFW& operator=(GLFW&&) = delete;

    static void init();

private:
    GLFW();
    ~GLFW();
};

} // namespace toto
