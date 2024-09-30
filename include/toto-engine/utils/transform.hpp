#pragma once

#include <glm/glm.hpp>

namespace toto {

class Transform {
public:
    void translate(const glm::vec3& translation) { _position += translation; }
    void rotate(const glm::vec3& rotation) { _rotation += rotation; }
    void scale(const glm::vec3& scale) { _scale *= scale; }
    void rotate(const glm::vec3& axis, float angle);
    void lookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec3& position() { return _position; }
    glm::vec3& rotation() { return _rotation; }
    glm::vec3& scale() { return _scale; }

    glm::vec3 position() const { return _position; }
    glm::vec3 rotation() const { return _rotation; }
    glm::vec3 scale() const { return _scale; }
    glm::mat4 matrix() const;

private:
    glm::vec3 _position = glm::vec3(0.0f);
    glm::vec3 _rotation = glm::vec3(0.0f);
    glm::vec3 _scale = glm::vec3(1.0f);
};

} // namespace toto
