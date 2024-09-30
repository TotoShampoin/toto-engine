#pragma once
#include "import-gl.hpp"
#include "toto-engine/gl/globjects.hpp"
#include "toto-engine/gl/glresources.hpp"
#include <functional>
#include <glm/glm.hpp>
#include <optional>
#include <vector>

namespace toto {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoord;
};

struct Model {
    GLBuffer<GLBufferTarget::Array> vbo;
    GLVertexArray<> vao;
    GLBuffer<GLBufferTarget::ElementArray> ibo;
    GLsizei index_count;

    Model(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
};
struct Material {
    glm::vec3 albedo;
    float alpha;
    float metallic;
    float roughness;
    float ao;
    glm::vec3 emissive;

    std::optional<std::reference_wrapper<GLTexture2D>> albedo_map;
    std::optional<std::reference_wrapper<GLTexture2D>> alpha_map;
    std::optional<std::reference_wrapper<GLTexture2D>> metallic_map;
    std::optional<std::reference_wrapper<GLTexture2D>> roughness_map;
    std::optional<std::reference_wrapper<GLTexture2D>> ao_map;
    std::optional<std::reference_wrapper<GLTexture2D>> emissive_map;
    std::optional<std::reference_wrapper<GLTexture2D>> normal_map;
    std::optional<std::reference_wrapper<GLTexture2D>> height_map;
};

std::string pbrMaterialVertexShader();
std::string pbrMaterialFragmentShader();

}; // namespace toto