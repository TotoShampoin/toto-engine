#pragma once

namespace toto {

class Clock {
public:
    Clock();
    ~Clock() = default;

    void start();
    void update();

    double deltaTime() const { return _delta_time; }
    double time() const { return _last_time - _start_time; }

private:
    double _start_time {0};
    double _last_time {0};
    double _delta_time {0};
};

} // namespace toto
