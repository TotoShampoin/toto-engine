#pragma once
#include <functional>
#include <toto-engine/import-gl.hpp>
#include <tuple>

namespace toto {

class WindowEvents {
public:
    void onResize(std::function<void(int, int)> callback);
    void onFramebufferResize(std::function<void(int, int)> callback);
    void onMouseButton(std::function<void(int, int, int, int)> callback);
    void onCursorPosition(std::function<void(double, double)> callback);
    void onScroll(std::function<void(double, double)> callback);
    void onKey(std::function<void(int, int, int, int)> callback);
    void onChar(std::function<void(int, int, int, int)> callback);
    void onCharMods(std::function<void(int, int, int, int)> callback);
    void onDrop(std::function<void(int, int)> callback);

private:
    void setCallbacks(GLFWwindow* window);

    std::function<void(int, int)> _resize_callback;
    std::function<void(int, int)> _framebuffer_resize_callback;
    std::function<void(int, int, int, int)> _mouse_button_callback;
    std::function<void(double, double)> _cursor_position_callback;
    std::function<void(double, double)> _scroll_callback;
    std::function<void(int, int, int, int)> _key_callback;
    std::function<void(int, int, int, int)> _char_callback;
    std::function<void(int, int, int, int)> _char_mods_callback;
    std::function<void(int, int)> _drop_callback;

    friend class Window;
};

class Window {
public:
    Window(int width, int height, const char* title, GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);
    ~Window();

    static void setHint(int hint, int value);

    static void initGL();

    void makeContextCurrent() const;

    static void pollEvents();
    void swapBuffers() const;
    bool shouldClose() const;

    std::tuple<int, int> size() const;

    GLFWwindow* handle() const;
    WindowEvents& events();

private:
    GLFWwindow* _handle;
    WindowEvents _events;
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
