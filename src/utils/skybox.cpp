#include "toto-engine/utils/skybox.hpp"
#include "toto-engine/gl/gldebug.hpp"
#include "toto-engine/gl/glresources.hpp"
#include "toto-engine/import-gl.hpp"
#include "toto-engine/loader/shader.hpp"
#include "toto-engine/uniform.hpp"
#include "toto-engine/utils/shapes.hpp"
#include <GL/gl.h>

namespace toto {

Skybox::Skybox(uint res)
    : _res(res) {
    GLDebug::pushGroup("Create skybox");

    _generateCubemaps();
    static_cast<void>(_cubemapModel());
    static_cast<void>(_cubemapProgram());
    static_cast<void>(skyboxCubemapModel());
    auto& program = skyboxProgram();
    auto& uniforms = skyboxUniforms();

    program.use();
    uniforms["u_cubemap"] = Uniform(program, "u_cubemap");
    uniforms["u_projection"] = Uniform(program, "u_projection");
    uniforms["u_view"] = Uniform(program, "u_view");
    uniforms["u_exposure"] = Uniform(program, "u_exposure");

    GLDebug::popGroup();
}

Skybox::Skybox(const GLTexture2D& hdri_texture, uint res)
    : Skybox(res) {
    GLDebug::pushGroup("Precompute skybox");

    GLFrameBuffer<>::bind(_framebuffer_cubemap);
    _renderbuffer_cubemap.storage(GL_DEPTH_COMPONENT24, _res, _res);
    _renderbuffer_cubemap.attach(GL_DEPTH_ATTACHMENT);
    GLFrameBuffer<>::bind(_framebuffer_irradiance);
    _renderbuffer_irradiance.storage(GL_DEPTH_COMPONENT24, 32, 32);
    _renderbuffer_irradiance.attach(GL_DEPTH_ATTACHMENT);
    GLFrameBuffer<>::bind(_framebuffer_prefiltered);
    _renderbuffer_prefiltered.storage(GL_DEPTH_COMPONENT24, 128, 128);
    _renderbuffer_prefiltered.attach(GL_DEPTH_ATTACHMENT);
    GLFrameBuffer<>::unbind();

    _renderToCubemap(hdri_texture);
    _renderIrradiance();
    _renderPrefiltered();

    GLDebug::popGroup();
}

void Skybox::applyCamera(const Camera& camera) const {
    auto& program = skyboxProgram();
    auto& uniforms = skyboxUniforms();

    program.use();
    uniforms["u_projection"].set(camera.projectionMatrix());
    uniforms["u_view"].set(camera.viewMatrix());
}
void Skybox::applyCubemap() const {
    auto& program = skyboxProgram();
    auto& uniforms = skyboxUniforms();
    auto& texture = _cubemap;
    // auto& texture = _prefiltered;
    // auto& texture = _irradiance;

    program.use();
    uniforms["u_cubemap"].set(texture, 0);
}
void Skybox::applyExposure(float exposure) const {
    auto& program = skyboxProgram();
    auto& uniforms = skyboxUniforms();

    program.use();
    uniforms["u_exposure"].set(exposure);
}

void Skybox::render(const Camera& camera) const {
    GLDebug::pushGroup("Render Skybox");

    auto& program = skyboxProgram();
    auto& model = skyboxCubemapModel();
    auto& uniforms = skyboxUniforms();
    auto& texture = _cubemap;

    glDepthFunc(GL_LEQUAL);
    model.vao.bind();
    // program.use();
    applyCubemap();
    applyCamera(camera);
    glDrawElements(GL_TRIANGLES, model.index_count, GL_UNSIGNED_INT, nullptr);
    model.vao.unbind();
    glDepthFunc(GL_LESS);

    GLDebug::popGroup();
}

void Skybox::_generateCubemaps() {
    GLDebug::pushGroup("Generate cubemaps");

    auto generate = [](GLTexture<GLTextureTarget::TextureCubeMap>& map, uint res, GLenum min_filter = GL_LINEAR,
                       GLenum mag_filter = GL_LINEAR) {
        for (int i = 0; i < 6; i++) {
            map.image2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, res, res, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        map.parameter(GL_TEXTURE_MIN_FILTER, min_filter);
        map.parameter(GL_TEXTURE_MAG_FILTER, mag_filter);
        map.parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        map.parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        map.parameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        map.generateMipmap();
    };
    generate(_cubemap, _res, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    generate(_irradiance, 32);
    generate(_prefiltered, 128, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    GLDebug::popGroup();
}

template <GLTextureTarget TARGET>
void renderCubemap(
    GLFrameBuffer<>& framebuffer, GLTexture<GLTextureTarget::TextureCubeMap>& output_cubemap,
    const GLTexture<TARGET>& input_texture, GLProgram& program, Uniform& u_texture, Uniform& u_projection,
    Uniform& u_view, int width, int height, int mip = 0
) {
    static auto camera = Camera::Perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    static std::tuple<glm::vec3, glm::vec3> camera_target_and_up[] = {
        { glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
        {glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
        { glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)},
        { glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)},
        { glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
        { glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
    };
    static auto& model = skyboxCubemapModel();

    framebuffer.bind();
    model.vao.bind();
    program.use();

    u_texture.set(input_texture, 0);
    u_projection.set(camera.projectionMatrix());

    glViewport(0, 0, width, height);
    for (int i = 0; i < 6; i++) {
        const auto& [camera_target, camera_up] = camera_target_and_up[i];
        camera.lookAt(camera_target, camera_up);
        u_view.set(camera.viewMatrix());

        framebuffer.texture2D(GL_COLOR_ATTACHMENT0, output_cubemap, mip, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, model.index_count, GL_UNSIGNED_INT, nullptr);
    }
    model.vao.unbind();
    framebuffer.unbind();
}

void Skybox::_renderToCubemap(const GLTexture2D& hdri_texture) {
    GLDebug::pushGroup("Cubemap");

    auto& program = _cubemapProgram();
    auto& model = _cubemapModel();
    auto& cubemap = _cubemap;
    auto& framebuffer = _framebuffer_cubemap;
    auto& renderbuffer = _renderbuffer_cubemap;

    auto u_hdri = Uniform(program, "u_hdri");
    auto u_projection = Uniform(program, "u_projection");
    auto u_view = Uniform(program, "u_view");

    renderCubemap(framebuffer, cubemap, hdri_texture, program, u_hdri, u_projection, u_view, _res, _res);
    cubemap.generateMipmap();

    GLDebug::popGroup();
}
void Skybox::_renderIrradiance() {
    GLDebug::pushGroup("Irradiance cubemap");

    auto& program = _irradianceProgram();
    auto& model = _cubemapModel();
    auto& cubemap = _cubemap;
    auto& irradiance = _irradiance;
    auto& framebuffer = _framebuffer_irradiance;
    auto& renderbuffer = _renderbuffer_irradiance;

    auto u_cubemap = Uniform(program, "u_cubemap");
    auto u_projection = Uniform(program, "u_projection");
    auto u_view = Uniform(program, "u_view");

    renderCubemap(framebuffer, irradiance, cubemap, program, u_cubemap, u_projection, u_view, 32, 32);

    GLDebug::popGroup();
}
void Skybox::_renderPrefiltered() {
    GLDebug::pushGroup("Prefiltered cubemap");

    auto& program = _prefilteredProgram();
    auto& model = _cubemapModel();
    auto& cubemap = _cubemap;
    auto& prefiltered = _prefiltered;
    auto& framebuffer = _framebuffer_prefiltered;
    auto& renderbuffer = _renderbuffer_prefiltered;

    program.use();

    auto u_cubemap = Uniform(program, "u_cubemap");
    auto u_projection = Uniform(program, "u_projection");
    auto u_view = Uniform(program, "u_view");
    auto u_roughness = Uniform(program, "u_roughness");

    framebuffer.bind();
    unsigned int max_mip_levels = 5;
    for (unsigned int mip = 0; mip < max_mip_levels; mip++) {
        unsigned int mip_width = 128 * std::pow(0.5, mip);
        unsigned int mip_height = 128 * std::pow(0.5, mip);

        renderbuffer.storage(GL_DEPTH_COMPONENT24, mip_width, mip_height);
        renderbuffer.attach(GL_DEPTH_ATTACHMENT);

        glViewport(0, 0, mip_width, mip_height);

        float roughness = (float)mip / (float)(max_mip_levels - 1);
        u_roughness.set(roughness);
        renderCubemap(
            framebuffer, prefiltered, cubemap, program, u_cubemap, u_projection, u_view, mip_width, mip_height, mip
        );
    }

    GLDebug::popGroup();
}

GLProgram& Skybox::skyboxProgram() {
    static GLProgram program = loadRenderShaderSource(skyboxVertexShader(), skyboxFragmentShader());
    return program;
}
Model& Skybox::skyboxModel() {
    static Model model = shape::cube();
    return model;
}
std::unordered_map<std::string, Uniform>& Skybox::skyboxUniforms() {
    static std::unordered_map<std::string, Uniform> uniforms;
    return uniforms;
}

Model& Skybox::_cubemapModel() {
    return skyboxCubemapModel();
}
GLProgram& Skybox::_cubemapProgram() {
    static GLProgram program = loadRenderShaderSource(cubemapVertexShader(), cubemapFragmentShader());
    return program;
}
GLProgram& Skybox::_irradianceProgram() {
    static GLProgram program = loadRenderShaderSource(cubemapVertexShader(), irradianceFragmentShader());
    return program;
}
GLProgram& Skybox::_prefilteredProgram() {
    static GLProgram program = loadRenderShaderSource(cubemapVertexShader(), prefilterFragmentShader());
    return program;
}

Model& skyboxCubemapModel() {
    static Model model = shape::cube(2.0f, 2.0f, 2.0f);
    return model;
}

}; // namespace toto
