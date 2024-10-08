#pragma once
#include "toto-engine/gl/glresources.hpp"
#include "toto-engine/mesh.hpp"
#include "toto-engine/uniform.hpp"
#include "toto-engine/utils/camera.hpp"
#include <string>
#include <unordered_map>

namespace toto {

std::string skyboxVertexShader();
std::string skyboxFragmentShader();
std::string cubemapVertexShader();
std::string cubemapFragmentShader();

// Nothing works *sad face*

class Skybox {
public:
    Skybox();
    Skybox(const GLTexture2D& hdri_texture);

    const GLTexture<GLTextureTarget::TextureCubeMap>& cubemap() const { return _cubemap; }

    void applyCamera(const Camera& camera) const;
    void applyCubemap() const;

    void render(const Camera& camera) const;

    friend class DeferredRenderer;

protected:
    static Model& skyboxModel();
    static GLProgram& skyboxProgram();
    static std::unordered_map<std::string, Uniform>& skyboxUniforms();

private:
    GLTexture<GLTextureTarget::TextureCubeMap> _cubemap;
    GLFrameBuffer<> _framebuffer;
    GLRenderBuffer<> _renderbuffer;

    void _renderToCubemap(const GLTexture2D& hdri_texture);
    void _generateCubemap();

    static GLProgram& _cubemapProgram();
    static Model& _cubemapModel();
};

} // namespace toto
