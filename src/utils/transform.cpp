#include "toto-engine/utils/transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

namespace toto {

void Transform::lookAt(const glm::vec3& target, const glm::vec3& up) {
    auto matrix = glm::lookAt(_position, target, up);
    matrix = glm::inverse(matrix);
    glm::extractEulerAngleXYZ(matrix, _rotation.x, _rotation.y, _rotation.z);
}

void Transform::rotate(const glm::vec3& axis, float angle) {
    glm::mat4 rotation_matrix = glm::rotate<float>(glm::mat4(1.0f), _rotation.x, {1.f, 0.f, 0.f});
    rotation_matrix = glm::rotate<float>(rotation_matrix, _rotation.y, {0.f, 1.f, 0.f});
    rotation_matrix = glm::rotate<float>(rotation_matrix, _rotation.z, {0.f, 0.f, 1.f});
    rotation_matrix = glm::rotate<float>(rotation_matrix, angle, axis);
    glm::extractEulerAngleXYZ(rotation_matrix, _rotation.x, _rotation.y, _rotation.z);
}

glm::mat4 Transform::matrix() const {
    glm::mat4 matrix = glm::translate(glm::mat4(1.0f), _position);
    matrix = matrix * glm::eulerAngleXYZ(_rotation.x, _rotation.y, _rotation.z);
    matrix = glm::scale(matrix, _scale);
    return matrix;
}

} // namespace toto
