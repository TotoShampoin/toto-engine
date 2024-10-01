#pragma once
#include "glpointer.hpp"
#include "toto-engine/import-gl.hpp"
#include <stdexcept>
#include <string>

namespace toto {

enum class GLShaderType {
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
    Geometry = GL_GEOMETRY_SHADER,
    TessControl = GL_TESS_CONTROL_SHADER,
    TessEvaluation = GL_TESS_EVALUATION_SHADER,
    Compute = GL_COMPUTE_SHADER
};

enum class GLTextureTarget {
    Texture1D = GL_TEXTURE_1D,
    Texture2D = GL_TEXTURE_2D,
    Texture3D = GL_TEXTURE_3D,
    Texture1DArray = GL_TEXTURE_1D_ARRAY,
    Texture2DArray = GL_TEXTURE_2D_ARRAY,
    TextureRectangle = GL_TEXTURE_RECTANGLE,
    TextureCubeMap = GL_TEXTURE_CUBE_MAP,
    TextureCubeMapArray = GL_TEXTURE_CUBE_MAP_ARRAY,
    Texture2DMultisample = GL_TEXTURE_2D_MULTISAMPLE,
    Texture2DMultisampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
};
enum class GLTextureCubemapTarget {
    PositiveX = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    NegativeX = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    PositiveY = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    NegativeY = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    PositiveZ = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    NegativeZ = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

template <GLShaderType SHADER_TYPE>
class GLShader : public GLPointer<
                     []() { return glCreateShader(static_cast<GLenum>(SHADER_TYPE)); },
                     [](GLuint shader) { glDeleteShader(shader); }> {
public:
    inline void source(const std::string& source) const {
        const char* c_source = source.c_str();
        glShaderSource(this->handle(), 1, &c_source, nullptr);
    }

    inline void compile() const {
        glCompileShader(this->handle());
        GLint success;
        glGetShaderiv(this->handle(), GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar info_log[512];
            glGetShaderInfoLog(this->handle(), 512, nullptr, info_log);
            throw std::runtime_error(info_log);
        }
    }
};

class GLProgram
    : public GLPointer<[]() { return glCreateProgram(); }, [](GLuint program) { glDeleteProgram(program); }> {
public:
    template <GLShaderType SHADER_TYPE>
    void attachShader(const GLShader<SHADER_TYPE>& shader) const {
        glAttachShader(this->handle(), shader.handle());
    }
    template <GLShaderType... SHADER_TYPES>
    void attachShaders(const GLShader<SHADER_TYPES>&... shaders) const {
        (attachShader(shaders), ...);
    }

    void link() const {
        glLinkProgram(this->handle());
        GLint success;
        glGetProgramiv(this->handle(), GL_LINK_STATUS, &success);
        if (!success) {
            GLchar info_log[512];
            glGetProgramInfoLog(this->handle(), 512, nullptr, info_log);
            throw std::runtime_error(info_log);
        }
    }

    static void use(const GLProgram& program) { glUseProgram(program.handle()); }
    static void unuse() { glUseProgram(0); }

    void use() const { use(*this); }
};

template <GLTextureTarget TARGET, GLsizei N = 1>
class GLTexture : public GLPointerArray<
                      [](GLsizei n, GLuint* textures) { glGenTextures(n, textures); },
                      [](GLsizei n, GLuint* textures) { glDeleteTextures(n, textures); }, N> {
public:
    inline static void bind(const GLTexture& texture, size_t offset = 0) {
        glBindTexture(static_cast<GLenum>(TARGET), texture.handle(offset));
    }
    inline static void unbind() { glBindTexture(static_cast<GLenum>(TARGET), 0); }

    inline void bind(size_t offset = 0) const { bind(*this, offset); }
    inline void active(uint index) const { glActiveTexture(GL_TEXTURE0 + index); }

    void parameter(GLenum parameter, GLint value) const {
        bind(*this);
        glTexParameteri(static_cast<GLenum>(TARGET), parameter, value);
    }

    void image2D(
        GLenum target, GLint level, GLint internal_format, GLsizei width, GLsizei height, GLint border, GLenum format,
        GLenum type, const void* data
    ) {
        bind(*this);
        glTexImage2D(target, level, internal_format, width, height, border, format, type, data);
    }
    void image3D(
        GLenum target, GLint level, GLint internal_format, GLsizei width, GLsizei height, GLsizei depth, GLint border,
        GLenum format, GLenum type, const void* data
    ) {
        bind(*this);
        glTexImage3D(target, level, internal_format, width, height, depth, border, format, type, data);
    }
};

using GLTexture2D = GLTexture<GLTextureTarget::Texture2D>;

template <GLsizei N = 1>
class GLFrameBuffer : public GLPointerArray<
                          [](GLsizei n, GLuint* framebuffers) { glGenFramebuffers(n, framebuffers); },
                          [](GLsizei n, GLuint* framebuffers) { glDeleteFramebuffers(n, framebuffers); }, N> {
public:
    static void bind(const GLFrameBuffer& framebuffer, size_t offset = 0) {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.handle(offset));
    }
    static void unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    void bind(size_t offset = 0) const { bind(*this, offset); }

    GLenum checkStatus() const {
        bind(*this);
        return glCheckFramebufferStatus(GL_FRAMEBUFFER);
    }

    void texture2D(GLenum attachment, const GLTexture2D& texture, GLint level) const {
        bind(*this);
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.handle(), level);
    }

private:
};

template <GLsizei N = 1>
class GLRenderBuffer : public GLPointerArray<
                           [](GLsizei n, GLuint* renderbuffers) { glGenRenderbuffers(n, renderbuffers); },
                           [](GLsizei n, GLuint* renderbuffers) { glDeleteRenderbuffers(n, renderbuffers); }, N> {
public:
    static void bind(const GLRenderBuffer& renderbuffer, size_t offset = 0) {
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer.handle(offset));
    }
    static void unbind() { glBindRenderbuffer(GL_RENDERBUFFER, 0); }

    void bind(size_t offset = 0) const { bind(*this, offset); }

    void storage(GLenum internal_format, GLsizei width, GLsizei height, size_t offset = 0) const {
        bind(*this, offset);
        glRenderbufferStorage(GL_RENDERBUFFER, internal_format, width, height);
    }

    void attach(GLenum attachment, size_t offset = 0) const {
        bind(*this, offset);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, this->handle());
    }
};

} // namespace toto
