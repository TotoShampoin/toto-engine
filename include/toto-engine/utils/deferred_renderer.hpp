#pragma once

#include "toto-engine/gl/glresources.hpp"
#include "toto-engine/loader/shader.hpp"
#include "toto-engine/mesh.hpp"
#include "toto-engine/uniform.hpp"
#include "toto-engine/utils/camera.hpp"
#include "toto-engine/utils/light.hpp"
#include "toto-engine/utils/shapes.hpp"
#include "toto-engine/utils/transform.hpp"
#include <functional>
#include <string>
#include <unordered_map>

namespace toto {

class DeferredRenderer {
public:
    DeferredRenderer();
    DeferredRenderer(int width, int height);

    void useDeferredProgram();
    void useLightingProgram();
    void enableDepthTest();
    void disableDepthTest();
    void clear(bool color = true, bool depth = true);
    void setViewport(int x, int y, int width, int height);
    void bindGBuffer();
    void unbindGBuffer();

    void setTransform(const Transform& transform);
    void setCamera(const Camera& camera);
    void setMaterial(const Material& material);
    void setLight(const Light& light);

    void draw(const Model& model);
    void draw(const Model& model, const Material& material, const Transform& transform);

    void beginRender();
    void endRender();

private:
    GLFrameBuffer<> _g_buffer;
    GLTexture<GLTextureTarget::Texture2D> _g_position;
    GLTexture<GLTextureTarget::Texture2D> _g_normal;
    GLTexture<GLTextureTarget::Texture2D> _g_albedo;
    GLTexture<GLTextureTarget::Texture2D> _g_metallic_roughness_ao;
    GLRenderBuffer<> _rboDepth;

    Model _quad = shape::quad(2.0f, 2.0f);

    GLProgram _deferred = loadRenderShaderSource(materialVertexShader(), deferredMaterialFragmentShader());
    GLProgram _lighting = loadRenderShaderSource(screenMaterialVertexShader(), lightingFragmentShader());
    std::unordered_map<std::string, Uniform> _uniforms_deferred;
    std::unordered_map<std::string, Uniform> _uniforms_lighting;

    void _setMap(const std::string& name, const std::optional<std::reference_wrapper<GLTexture2D>>& map, uint index);
};

} // namespace toto
