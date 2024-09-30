#include "toto-engine/loader/image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace toto {

Image2D<unsigned char> loadImage2D(const std::filesystem::path& path) {
    Image2D<unsigned char> image;
    stbi_set_flip_vertically_on_load(true);
    auto data = stbi_load(path.c_str(), &image.width, &image.height, &image.channels, 0);
    if (!data) {
        throw std::runtime_error("Failed to load image");
    }
    image.data.reserve(image.width * image.height * image.channels);
    image.data.insert(image.data.end(), data, data + image.width * image.height * image.channels);
    return image;
}

Image2D<float> loadImage2Df(const std::filesystem::path& path) {
    Image2D<float> image;
    stbi_set_flip_vertically_on_load(true);
    auto data = stbi_loadf(path.c_str(), &image.width, &image.height, &image.channels, 0);
    if (!data) {
        throw std::runtime_error("Failed to load image");
    }
    image.data.reserve(image.width * image.height * image.channels);
    image.data.insert(image.data.end(), data, data + image.width * image.height * image.channels);
    return image;
}

} // namespace toto