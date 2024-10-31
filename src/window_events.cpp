#include "toto-engine/window.hpp"

namespace toto {

void WindowEvents::onResize(std::function<void(int, int)> callback) {
    _resize_callback = callback;
}
void WindowEvents::onFramebufferResize(std::function<void(int, int)> callback) {
    _framebuffer_resize_callback = callback;
}
void WindowEvents::onMouseButton(std::function<void(int, int, int, int)> callback) {
    _mouse_button_callback = callback;
}
void WindowEvents::onCursorPosition(std::function<void(double, double)> callback) {
    _cursor_position_callback = callback;
}
void WindowEvents::onScroll(std::function<void(double, double)> callback) {
    _scroll_callback = callback;
}
void WindowEvents::onKey(std::function<void(int, int, int, int)> callback) {
    _key_callback = callback;
}
void WindowEvents::onChar(std::function<void(int, int, int, int)> callback) {
    _char_callback = callback;
}
void WindowEvents::onCharMods(std::function<void(int, int, int, int)> callback) {
    _char_mods_callback = callback;
}
void WindowEvents::onDrop(std::function<void(int, int)> callback) {
    _drop_callback = callback;
}

void WindowEvents::setCallbacks(GLFWwindow* window) {
    glfwSetWindowUserPointer(window, this);
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        auto events = static_cast<WindowEvents*>(glfwGetWindowUserPointer(window));
        if (events->_resize_callback) {
            events->_resize_callback(width, height);
        }
    });
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        auto events = static_cast<WindowEvents*>(glfwGetWindowUserPointer(window));
        if (events->_framebuffer_resize_callback) {
            events->_framebuffer_resize_callback(width, height);
        }
    });
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
        auto events = static_cast<WindowEvents*>(glfwGetWindowUserPointer(window));
        if (events->_mouse_button_callback) {
            events->_mouse_button_callback(button, action, mods, 0);
        }
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
        auto events = static_cast<WindowEvents*>(glfwGetWindowUserPointer(window));
        if (events->_cursor_position_callback) {
            events->_cursor_position_callback(x, y);
        }
    });
    glfwSetScrollCallback(window, [](GLFWwindow* window, double x, double y) {
        auto events = static_cast<WindowEvents*>(glfwGetWindowUserPointer(window));
        if (events->_scroll_callback) {
            events->_scroll_callback(x, y);
        }
    });
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto events = static_cast<WindowEvents*>(glfwGetWindowUserPointer(window));
        if (events->_key_callback) {
            events->_key_callback(key, scancode, action, mods);
        }
    });
    glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int codepoint) {
        auto events = static_cast<WindowEvents*>(glfwGetWindowUserPointer(window));
        if (events->_char_callback) {
            events->_char_callback(codepoint, 0, 0, 0);
        }
    });
    glfwSetCharModsCallback(window, [](GLFWwindow* window, unsigned int codepoint, int mods) {
        auto events = static_cast<WindowEvents*>(glfwGetWindowUserPointer(window));
        if (events->_char_mods_callback) {
            events->_char_mods_callback(codepoint, mods, 0, 0);
        }
    });
    glfwSetDropCallback(window, [](GLFWwindow* window, int count, const char** paths) {
        auto events = static_cast<WindowEvents*>(glfwGetWindowUserPointer(window));
        if (events->_drop_callback) {
            events->_drop_callback(count, 0);
        }
    });
}

} // namespace toto
