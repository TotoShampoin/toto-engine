#pragma once
#include "toto-engine/import-gl.hpp"

namespace toto {

using GLConstructorType = GLuint (*)();
using GLDestructorType = void (*)(GLuint);
using GLConstructorTypeArray = void (*)(GLsizei, GLuint*);
using GLDestructorTypeArray = void (*)(GLsizei, GLuint*);

template <GLConstructorType construct, GLDestructorType destruct>
class GLPointer {
public:
    GLPointer() { _handle = construct(); }
    ~GLPointer() { destruct(_handle); }
    GLPointer(GLPointer&& other) {
        _handle = other._handle;
        other._handle = 0;
    }
    GLPointer& operator=(GLPointer&& other) {
        if (this == &other)
            return *this;
        destruct(_handle);
        _handle = other._handle;
        other._handle = 0;
        return *this;
    }
    GLPointer(const GLPointer&) = delete;
    GLPointer& operator=(const GLPointer&) = delete;

    inline GLuint handle() const { return _handle; }
    inline GLuint operator*() const { return _handle; }

    operator GLuint() const { return _handle; }

private:
    GLuint _handle {0};
};

template <GLConstructorTypeArray construct, GLDestructorTypeArray destruct, GLsizei N>
class GLPointerArray {
public:
    GLPointerArray() { construct(N, &_handle); }
    ~GLPointerArray() { destruct(N, &_handle); }
    GLPointerArray(GLPointerArray&& other) {
        _handle = other._handle;
        other._handle = 0;
    }
    GLPointerArray& operator=(GLPointerArray&& other) {
        if (this == &other)
            return *this;
        destruct(1, &_handle);
        _handle = other._handle;
        other._handle = 0;
        return *this;
    }
    GLPointerArray(const GLPointerArray&) = delete;
    GLPointerArray& operator=(const GLPointerArray&) = delete;

    inline GLuint handle() const { return _handle; }
    inline GLuint operator*() const { return _handle; }

    operator GLuint() const { return _handle; }

private:
    GLuint _handle {0};
};

} // namespace toto
