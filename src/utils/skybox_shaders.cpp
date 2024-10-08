#include "toto-engine/utils/skybox.hpp"

#include "../embed/skybox.frag.cpp"
#include "../embed/skybox.vert.cpp"
#include "../embed/skybox_cubemap.frag.cpp"
#include "../embed/skybox_cubemap.vert.cpp"

namespace toto {

std::string skyboxVertexShader() {
    std::vector<char> data(EMBED_SKYBOX_VERT.begin(), EMBED_SKYBOX_VERT.end());
    data.push_back('\0');
    return std::string(data.data());
}

std::string skyboxFragmentShader() {
    std::vector<char> data(EMBED_SKYBOX_FRAG.begin(), EMBED_SKYBOX_FRAG.end());
    data.push_back('\0');
    return std::string(data.data());
}

std::string cubemapVertexShader() {
    std::vector<char> data(EMBED_SKYBOX_CUBEMAP_VERT.begin(), EMBED_SKYBOX_CUBEMAP_VERT.end());
    data.push_back('\0');
    return std::string(data.data());
}

std::string cubemapFragmentShader() {
    std::vector<char> data(EMBED_SKYBOX_CUBEMAP_FRAG.begin(), EMBED_SKYBOX_CUBEMAP_FRAG.end());
    data.push_back('\0');
    return std::string(data.data());
}

} // namespace toto