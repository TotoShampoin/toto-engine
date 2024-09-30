#include "toto-engine/utils/camera.hpp"
#include <glm/ext/matrix_clip_space.hpp>

namespace toto {

Camera::Camera(const glm::mat4& projection_matrix)
    : _projection_matrix(projection_matrix) {}

Camera Camera::Perspective(float fov, float aspect, float near, float far) {
    return Camera(glm::perspective(fov, aspect, near, far));
}

Camera Camera::Orthographic(float left, float right, float bottom, float top, float near, float far) {
    return Camera(glm::ortho(left, right, bottom, top, near, far));
}

void Camera::setProjectionMatrix(const glm::mat4& projection_matrix) {
    _projection_matrix = projection_matrix;
}

void Camera::setPerspective(float fov, float aspect, float near, float far) {
    _projection_matrix = glm::perspective(fov, aspect, near, far);
}

void Camera::setOrthographic(float left, float right, float bottom, float top, float near, float far) {
    _projection_matrix = glm::ortho(left, right, bottom, top, near, far);
}

void Camera::lookAt(const glm::vec3& target, const glm::vec3& up) {
    _transform.lookAt(target, up);
}

} // namespace toto
