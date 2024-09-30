#include "toto-engine/mesh.hpp"

#include "embed/pbr.frag.cpp"
#include "embed/pbr.vert.cpp"

namespace toto {

std::string pbrMaterialVertexShader() {
    std::vector<char> data(EMBED_PBR_VERT.begin(), EMBED_PBR_VERT.end());
    data.push_back('\0');
    return std::string(data.data());
}

std::string pbrMaterialFragmentShader() {
    std::vector<char> data(EMBED_PBR_FRAG.begin(), EMBED_PBR_FRAG.end());
    data.push_back('\0');
    return std::string(data.data());
}

}; // namespace toto
