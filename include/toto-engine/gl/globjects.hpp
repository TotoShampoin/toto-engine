#pragma once
#include "glpointer.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace toto {

enum class GLBufferTarget {
    Array = GL_ARRAY_BUFFER,
    ElementArray = GL_ELEMENT_ARRAY_BUFFER,
    Uniform = GL_UNIFORM_BUFFER,
    ShaderStorage = GL_SHADER_STORAGE_BUFFER
};

inline void genBuffers(GLsizei n, GLuint* buffers) {
    glGenBuffers(n, buffers);
}
inline void deleteBuffers(GLsizei n, GLuint* buffers) {
    glDeleteBuffers(n, buffers);
}
inline void genVertexArrays(GLsizei n, GLuint* arrays) {
    glGenVertexArrays(n, arrays);
}
inline void deleteVertexArrays(GLsizei n, GLuint* arrays) {
    glDeleteVertexArrays(n, arrays);
}

template <GLBufferTarget TARGET, GLsizei N = 1>
class GLBuffer : public GLPointerArray<genBuffers, deleteBuffers, N> {
public:
    inline static void bind(const GLBuffer& buffer, size_t offset = 0) {
        glBindBuffer(static_cast<GLenum>(TARGET), buffer.handle(offset));
    }
    inline static void unbind() { glBindBuffer(static_cast<GLenum>(TARGET), 0); }

    inline void bind(size_t offset = 0) const { bind(*this, offset); }

    template <typename T>
    void data(const std::vector<T>& data, GLenum usage) const {
        bind(*this);
        glBufferData(static_cast<GLenum>(TARGET), data.size() * sizeof(T), data.data(), usage);
    }

    template <typename T>
    void subData(const std::vector<T>& data, size_t offset) const {
        bind(*this);
        glBufferSubData(static_cast<GLenum>(TARGET), offset, data.size() * sizeof(T), data.data());
    }
};

template <GLsizei N = 1>
class GLVertexArray : public GLPointerArray<genVertexArrays, deleteVertexArrays, N> {
public:
    inline static void bind(const GLVertexArray& array, size_t offset = 0) { glBindVertexArray(array.handle(offset)); }
    inline static void unbind() { glBindVertexArray(0); }

    inline void bind(size_t offset = 0) const { bind(*this, offset); }

    /**
     * @brief Set the Vertex Attrib Pointer
     *
     * @tparam VAOType The struct that you are using
     * @tparam SubType The number type for the attribute
     * @tparam SIZE The size of the attribute
     * @tparam OFFSET The offset of the attribute
     * @param index
     */
    template <typename VAOType, typename SubType, size_t SIZE, size_t OFFSET>
    void setVertexAttrib(GLuint index) const {
        bind();
        glEnableVertexAttribArray(index);
        if constexpr (std::is_same_v<SubType, double>) {
            glVertexAttribPointer(index, SIZE, GL_DOUBLE, GL_FALSE, sizeof(VAOType), (void*)OFFSET);
        } else if constexpr (std::is_same_v<SubType, float>) {
            glVertexAttribPointer(index, SIZE, GL_FLOAT, GL_FALSE, sizeof(VAOType), (void*)OFFSET);
        } else if constexpr (std::is_same_v<SubType, int>) {
            glVertexAttribIPointer(index, SIZE, GL_INT, sizeof(VAOType), (void*)OFFSET);
        } else if constexpr (std::is_same_v<SubType, unsigned int>) {
            glVertexAttribIPointer(index, SIZE, GL_UNSIGNED_INT, sizeof(VAOType), (void*)OFFSET);
        } else {
            static_assert(false, "Invalid SubType");
        }
    }

    /**
     * @brief Set the Vertex Attrib Pointer
     *
     * @tparam VAOType The struct that you are using
     * @tparam AttrType The glm::vec type for the attribute.
     * @tparam OFFSET The offset of the attribute
     * @param index
     */
    template <typename VAOType, typename AttrType, size_t OFFSET>
    void setAttribPointer(GLuint index) const {
        constexpr auto size = std::is_same_v<AttrType, glm::vec1>   ? 1
                              : std::is_same_v<AttrType, glm::vec2> ? 2
                              : std::is_same_v<AttrType, glm::vec3> ? 3
                              : std::is_same_v<AttrType, glm::vec4> ? 4
                                                                    : 1;

        bind();
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, sizeof(VAOType), (void*)OFFSET);
    }

    void setAttribDivisor(GLuint index, GLuint divisor) const {
        bind();
        glVertexAttribDivisor(index, divisor);
    }
};

} // namespace toto
