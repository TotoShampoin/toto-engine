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
std::string irradianceFragmentShader();

class Skybox {
public:
    Skybox(uint res = 512);
    Skybox(const GLTexture2D& hdri_texture, uint res = 512);

    const GLTexture<GLTextureTarget::TextureCubeMap>& cubemap() const { return _cubemap; }
    const GLTexture<GLTextureTarget::TextureCubeMap>& irradiance() const { return _irradiance; }

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
    GLTexture<GLTextureTarget::TextureCubeMap> _irradiance;
    GLFrameBuffer<> _framebuffer_cubemap;
    GLRenderBuffer<> _renderbuffer_cubemap;
    GLFrameBuffer<> _framebuffer_irradiance;
    GLRenderBuffer<> _renderbuffer_irradiance;
    uint _res;

    void _renderToCubemap(const GLTexture2D& hdri_texture);
    void _generateCubemaps();
    void _renderIrradiance();

    static Model& _cubemapModel();
    static GLProgram& _cubemapProgram();
    static GLProgram& _irradianceProgram();
};

} // namespace toto
