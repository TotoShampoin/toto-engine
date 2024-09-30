#pragma once

#include "toto-engine/gl/glresources.hpp"
#include "toto-engine/loader/shader.hpp"
#include "toto-engine/mesh.hpp"
#include "toto-engine/uniform.hpp"
#include "toto-engine/utils/camera.hpp"
#include "toto-engine/utils/light.hpp"
#include "toto-engine/utils/transform.hpp"
#include <string>
#include <unordered_map>

namespace toto {

class Renderer {
public:
    Renderer();

    void useProgram();
    void enableDepthTest();
    void disableDepthTest();
    void clear(bool color = true, bool depth = true);
    void setViewport(int x, int y, int width, int height);

    void setModelMatrix(const glm::mat4& model_matrix);
    void setViewMatrix(const glm::mat4& view_matrix);
    void setProjectionMatrix(const glm::mat4& projection_matrix);

    void setTransform(const Transform& transform);
    void setCamera(const Camera& camera);
    void setMaterial(const Material& material);
    void setLight(const Light& light);

    void render(const Model& model);

    void render(const Model& model, const Material& material, const Transform& transform);

private:
    GLProgram _program = loadRenderShaderSource(pbrMaterialVertexShader(), pbrMaterialFragmentShader());
    std::unordered_map<std::string, Uniform> _uniforms;

    void _setMap(const std::string& name, const std::optional<std::reference_wrapper<GLTexture2D>>& map, uint index);
};

} // namespace toto
