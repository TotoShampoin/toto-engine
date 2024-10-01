#include "toto-engine/mesh.hpp"

#include "embed/deferred.frag.cpp"
#include "embed/deferred_pbr.frag.cpp"
#include "embed/pbr.frag.cpp"
#include "embed/screen.vert.cpp"
#include "embed/vertex.vert.cpp"

namespace toto {

std::string materialVertexShader() {
    std::vector<char> data(EMBED_VERTEX_VERT.begin(), EMBED_VERTEX_VERT.end());
    data.push_back('\0');
    return std::string(data.data());
}

std::string screenMaterialVertexShader() {
    std::vector<char> data(EMBED_SCREEN_VERT.begin(), EMBED_SCREEN_VERT.end());
    data.push_back('\0');
    return std::string(data.data());
}

std::string pbrMaterialFragmentShader() {
    std::vector<char> data(EMBED_PBR_FRAG.begin(), EMBED_PBR_FRAG.end());
    data.push_back('\0');
    return std::string(data.data());
}

std::string deferredMaterialFragmentShader() {
    std::vector<char> data(EMBED_DEFERRED_FRAG.begin(), EMBED_DEFERRED_FRAG.end());
    data.push_back('\0');
    return std::string(data.data());
}

std::string lightingFragmentShader() {
    std::vector<char> data(EMBED_DEFERRED_PBR_FRAG.begin(), EMBED_DEFERRED_PBR_FRAG.end());
    data.push_back('\0');
    return std::string(data.data());
}

}; // namespace toto
