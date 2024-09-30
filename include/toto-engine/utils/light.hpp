#pragma once

#include "transform.hpp"

namespace toto {

class Light {
public:
    Light(const glm::vec3& color, float intensity, bool is_directional = true)
        : _transform(),
          _color(color),
          _intensity(intensity),
          _is_directional(is_directional) {}

    Transform& transform() { return _transform; }
    glm::vec3& color() { return _color; }
    float& intensity() { return _intensity; }
    bool& isDirectional() { return _is_directional; }

    const Transform& transform() const { return _transform; }
    glm::vec3 direction() const {
        return glm::normalize(glm::vec3(_transform.matrix() * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
    }
    glm::vec3 color() const { return _color; }
    float intensity() const { return _intensity; }
    bool isDirectional() const { return _is_directional; }

private:
    Transform _transform;
    glm::vec3 _color;
    float _intensity;
    bool _is_directional;
};

} // namespace toto
