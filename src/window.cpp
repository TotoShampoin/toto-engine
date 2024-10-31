#include <expected>
#include <format>
#include <stdexcept>

#include "toto-engine/window.hpp"

namespace toto {

Window::Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share) {
    GLFW::init();
    _handle = glfwCreateWindow(width, height, title, monitor, share);
    if (!_handle)
        throw std::runtime_error("Failed to create GLFW window\n");
    _events.setCallbacks(_handle);
}
Window::~Window() {
    glfwDestroyWindow(_handle);
}

void Window::setHint(int hint, int value) {
    glfwWindowHint(hint, value);
}

void Window::initGL() {
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return;
    throw std::runtime_error("Failed to initialize GLAD\n");
}

void Window::makeContextCurrent() const {
    glfwMakeContextCurrent(_handle);
}

void Window::pollEvents() {
    glfwPollEvents();
}
void Window::swapBuffers() const {
    glfwSwapBuffers(_handle);
}
bool Window::shouldClose() const {
    return glfwWindowShouldClose(_handle);
}

std::tuple<int, int> Window::size() const {
    int width, height;
    glfwGetWindowSize(_handle, &width, &height);
    return {width, height};
}

GLFWwindow* Window::handle() const {
    return _handle;
}

WindowEvents& Window::events() {
    return _events;
}

GLFW::GLFW() {
    if (glfwInit()) {
        return;
    }
    char error_cstr[1024];
    glfwGetError(reinterpret_cast<const char**>(&error_cstr));
    throw std::runtime_error(std::format("Failed to initialize GLFW: {}\n", error_cstr));
}
GLFW::~GLFW() {
    glfwTerminate();
}
void GLFW::init() {
    static GLFW instance;
}

} // namespace toto