#include "toto-engine/utils/clock.hpp"
#include <GLFW/glfw3.h>

namespace toto {

Clock::Clock() {
    start();
}

void Clock::start() {
    _start_time = glfwGetTime();
    _last_time = _start_time;
}

void Clock::update() {
    auto now = glfwGetTime();
    _delta_time = now - _last_time;
    _last_time = now;
}

} // namespace toto
