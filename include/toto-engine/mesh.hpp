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
    std::string name;

    Model(
        const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices,
        const std::string& name = "Unnamed model"
    );
    ~Model();
};
struct Material {
    glm::vec3 albedo = {1.0f, 1.0f, 1.0f};
    float alpha = 1.0f;
    float metallic = 0.0f;
    float roughness = 0.5f;
    float ao = 1.0f;
    glm::vec3 emissive = {0.0f, 0.0f, 0.0f};
    std::string name = "Unnamed material";

    std::optional<std::reference_wrapper<GLTexture2D>> albedo_map;
    std::optional<std::reference_wrapper<GLTexture2D>> alpha_map;
    std::optional<std::reference_wrapper<GLTexture2D>> metallic_map;
    std::optional<std::reference_wrapper<GLTexture2D>> roughness_map;
    std::optional<std::reference_wrapper<GLTexture2D>> ao_map;
    std::optional<std::reference_wrapper<GLTexture2D>> emissive_map;
    std::optional<std::reference_wrapper<GLTexture2D>> normal_map;
    std::optional<std::reference_wrapper<GLTexture2D>> height_map;
};

std::string materialVertexShader();
std::string screenMaterialVertexShader();
std::string pbrMaterialFragmentShader();
std::string deferredMaterialFragmentShader();
std::string lightingFragmentShader();

} // namespace toto