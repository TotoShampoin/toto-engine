#pragma once
#include "glpointer.hpp"
#include "toto-engine/import-gl.hpp"
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

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
    inline static void bind(const GLTexture& texture) { glBindTexture(static_cast<GLenum>(TARGET), texture.handle()); }
    inline static void unbind() { glBindTexture(static_cast<GLenum>(TARGET), 0); }

    inline void bind() const { bind(*this); }
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

} // namespace toto
