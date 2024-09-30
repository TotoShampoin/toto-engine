#pragma once

#include "toto-engine/gl/glresources.hpp"
#include <glm/glm.hpp>

namespace toto {

class Uniform {
public:
    Uniform()
        : _handle(0) {}
    Uniform(const GLProgram& program, const std::string& name) {
        _handle = glGetUniformLocation(program.handle(), name.c_str());
    }

    void set(bool value) const { glUniform1i(_handle, value); }
    void set(int value) const { glUniform1i(_handle, value); }
    void set(uint value) const { glUniform1ui(_handle, value); }
    void set(float value) const { glUniform1f(_handle, value); }
    void set(double value) const { glUniform1d(_handle, value); }
    void set(const glm::vec2& value) const { glUniform2fv(_handle, 1, &value[0]); }
    void set(const glm::vec3& value) const { glUniform3fv(_handle, 1, &value[0]); }
    void set(const glm::vec4& value) const { glUniform4fv(_handle, 1, &value[0]); }
    void set(const glm::ivec2& value) const { glUniform2iv(_handle, 1, &value[0]); }
    void set(const glm::ivec3& value) const { glUniform3iv(_handle, 1, &value[0]); }
    void set(const glm::ivec4& value) const { glUniform4iv(_handle, 1, &value[0]); }
    void set(const glm::mat2& value) const { glUniformMatrix2fv(_handle, 1, GL_FALSE, &value[0][0]); }
    void set(const glm::mat3& value) const { glUniformMatrix3fv(_handle, 1, GL_FALSE, &value[0][0]); }
    void set(const glm::mat4& value) const { glUniformMatrix4fv(_handle, 1, GL_FALSE, &value[0][0]); }
    void set(const glm::mat2x3& value) const { glUniformMatrix2x3fv(_handle, 1, GL_FALSE, &value[0][0]); }
    void set(const glm::mat3x2& value) const { glUniformMatrix3x2fv(_handle, 1, GL_FALSE, &value[0][0]); }
    void set(const glm::mat2x4& value) const { glUniformMatrix2x4fv(_handle, 1, GL_FALSE, &value[0][0]); }
    void set(const glm::mat4x2& value) const { glUniformMatrix4x2fv(_handle, 1, GL_FALSE, &value[0][0]); }
    void set(const glm::mat3x4& value) const { glUniformMatrix3x4fv(_handle, 1, GL_FALSE, &value[0][0]); }
    void set(const glm::mat4x3& value) const { glUniformMatrix4x3fv(_handle, 1, GL_FALSE, &value[0][0]); }

    void set(const GLTexture2D& texture, int unit) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        texture.bind();
        glUniform1i(_handle, unit);
    }

private:
    uint _handle;
};

}; // namespace toto
