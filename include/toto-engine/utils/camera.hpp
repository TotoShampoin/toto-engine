#pragma once

#include "transform.hpp"

namespace toto {

class Camera {
public:
    Camera() = default;
    Camera(const glm::mat4& projection_matrix);
    static Camera Perspective(float fov, float aspect, float near, float far);
    static Camera Orthographic(float left, float right, float bottom, float top, float near, float far);

    void setProjectionMatrix(const glm::mat4& projection_matrix);
    void setPerspective(float fov, float aspect, float near, float far);
    void setOrthographic(float left, float right, float bottom, float top, float near, float far);

    Transform& transform() { return _transform; }
    void lookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 projectionMatrix() const { return _projection_matrix; }
    glm::mat4 viewMatrix() const { return glm::inverse(_transform.matrix()); }

private:
    Transform _transform;
    glm::mat4 _projection_matrix;
};

} // namespace toto
