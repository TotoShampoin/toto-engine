#pragma once

#include "toto-engine/gl/glresources.hpp"
#include <filesystem>
#include <vector>

namespace toto {

template <typename TYPE>
struct Image2D {
    std::vector<TYPE> data;
    int width;
    int height;
    int channels;
};

struct TextureParameters {
    GLenum wrap_s = GL_REPEAT;
    GLenum wrap_t = GL_REPEAT;
    GLenum min_filter = GL_LINEAR;
    GLenum mag_filter = GL_LINEAR;
};

Image2D<unsigned char> loadImage2D(const std::filesystem::path& path);

Image2D<float> loadImage2Df(const std::filesystem::path& path);

template <typename TYPE>
GLTexture2D generateTexture2D(const Image2D<TYPE>& image, const TextureParameters& parameters = {}) {
    GLTexture2D texture;
    const GLenum format = [&] {
        switch (image.channels) {
        case 1: return GL_RED;
        case 2: return GL_RG;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        default: throw std::runtime_error("Invalid number of channels");
        }
    }();
    const GLenum internal_format = [&] {
        switch (image.channels) {
        case 1: return std::is_same_v<TYPE, unsigned char> ? GL_R8 : GL_R16F;
        case 2: return std::is_same_v<TYPE, unsigned char> ? GL_RG8 : GL_RG16F;
        case 3: return std::is_same_v<TYPE, unsigned char> ? GL_RGB8 : GL_RGB16F;
        case 4: return std::is_same_v<TYPE, unsigned char> ? GL_RGBA8 : GL_RGBA16F;
        default: throw std::runtime_error("Invalid number of channels");
        }
    }();
    const GLenum type = std::is_same_v<TYPE, unsigned char> ? GL_UNSIGNED_BYTE : GL_FLOAT;
    // const GLenum internal_format = format;
    texture.bind();
    texture.image2D(GL_TEXTURE_2D, 0, internal_format, image.width, image.height, 0, format, type, image.data.data());
    texture.parameter(GL_TEXTURE_WRAP_S, parameters.wrap_s);
    texture.parameter(GL_TEXTURE_WRAP_T, parameters.wrap_t);
    texture.parameter(GL_TEXTURE_MIN_FILTER, parameters.min_filter);
    texture.parameter(GL_TEXTURE_MAG_FILTER, parameters.mag_filter);
    return texture;
}

inline GLTexture2D loadTexture2D(const std::filesystem::path& path, const TextureParameters& parameters = {}) {
    return generateTexture2D(loadImage2D(path), parameters);
}
inline GLTexture2D loadTexture2Df(const std::filesystem::path& path, const TextureParameters& parameters = {}) {
    return generateTexture2D(loadImage2Df(path), parameters);
}

} // namespace toto
