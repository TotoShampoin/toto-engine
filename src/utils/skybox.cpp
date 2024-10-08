#include "toto-engine/utils/skybox.hpp"
#include "toto-engine/gl/glresources.hpp"
#include "toto-engine/import-gl.hpp"
#include "toto-engine/loader/shader.hpp"
#include "toto-engine/uniform.hpp"
#include "toto-engine/utils/shapes.hpp"
#include <GL/gl.h>

namespace toto {

Skybox::Skybox(uint res)
    : _res(res) {
    _generateCubemaps();
    static_cast<void>(_cubemapModel());
    static_cast<void>(_cubemapProgram());
    static_cast<void>(skyboxModel());
    auto& program = skyboxProgram();
    auto& uniforms = skyboxUniforms();

    program.use();
    uniforms["u_cubemap"] = Uniform(program, "u_cubemap");
    uniforms["u_projection"] = Uniform(program, "u_projection");
    uniforms["u_view"] = Uniform(program, "u_view");
}

Skybox::Skybox(const GLTexture2D& hdri_texture, uint res)
    : Skybox(res) {
    GLFrameBuffer<>::bind(_framebuffer_cubemap);
    _renderbuffer_cubemap.storage(GL_DEPTH_COMPONENT24, _res, _res);
    _renderbuffer_cubemap.attach(GL_DEPTH_ATTACHMENT);
    GLFrameBuffer<>::bind(_framebuffer_irradiance);
    _renderbuffer_irradiance.storage(GL_DEPTH_COMPONENT24, 32, 32);
    _renderbuffer_irradiance.attach(GL_DEPTH_ATTACHMENT);
    GLFrameBuffer<>::unbind();

    _renderToCubemap(hdri_texture);
    _renderIrradiance();
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

    program.use();
    uniforms["u_cubemap"].set(texture, 0);
}

void Skybox::render(const Camera& camera) const {
    auto& program = skyboxProgram();
    auto& model = skyboxModel();
    auto& uniforms = skyboxUniforms();
    auto& texture = _cubemap;

    glDepthFunc(GL_LEQUAL);
    model.vao.bind();
    // program.use();
    applyCubemap();
    applyCamera(camera);
    glDrawElements(GL_TRIANGLES, model.index_count, GL_UNSIGNED_INT, nullptr);
    model.vao.unbind();
}

void Skybox::_generateCubemaps() {
    auto& cubemap = _cubemap;
    auto& irradiance = _irradiance;
    auto generate = [](GLTexture<GLTextureTarget::TextureCubeMap>& map, uint res) {
        map.parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        map.parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        map.parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        map.parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        map.parameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        for (int i = 0; i < 6; i++) {
            map.image2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, res, res, 0, GL_RGB, GL_FLOAT, nullptr);
        }
    };
    generate(cubemap, _res);
    generate(irradiance, 32);
}
void Skybox::_renderToCubemap(const GLTexture2D& hdri_texture) {
    auto& program = _cubemapProgram();
    auto& model = _cubemapModel();
    auto& cubemap = _cubemap;
    auto& framebuffer = _framebuffer_cubemap;
    auto& renderbuffer = _renderbuffer_cubemap;

    auto camera = Camera::Perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    std::tuple<glm::vec3, glm::vec3> camera_target_and_up[] = {
        { glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
        {glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
        { glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)},
        { glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)},
        { glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
        { glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
    };

    framebuffer.bind();
    model.vao.bind();
    program.use();
    Uniform(program, "u_hdri").set(hdri_texture, 0);
    Uniform(program, "u_projection").set(camera.projectionMatrix());
    auto u_view = Uniform(program, "u_view");
    glViewport(0, 0, _res, _res);
    for (int i = 0; i < 6; i++) {
        const auto& [camera_target, camera_up] = camera_target_and_up[i];
        camera.lookAt(camera_target, camera_up);
        u_view.set(camera.viewMatrix());

        framebuffer.texture2D(GL_COLOR_ATTACHMENT0, cubemap, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, model.index_count, GL_UNSIGNED_INT, nullptr);
    }
    model.vao.unbind();
    framebuffer.unbind();
}
// TODO: DRY
void Skybox::_renderIrradiance() {
    auto& program = _irradianceProgram();
    auto& model = _cubemapModel();
    auto& cubemap = _cubemap;
    auto& irradiance = _irradiance;
    auto& framebuffer = _framebuffer_irradiance;
    auto& renderbuffer = _renderbuffer_irradiance;

    auto camera = Camera::Perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    std::tuple<glm::vec3, glm::vec3> camera_target_and_up[] = {
        { glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
        {glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
        { glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)},
        { glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)},
        { glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
        { glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)},
    };

    model.vao.bind();
    framebuffer.bind();
    program.use();
    Uniform(program, "u_cubemap").set(cubemap, 0);
    Uniform(program, "u_projection").set(camera.projectionMatrix());
    auto u_view = Uniform(program, "u_view");
    glViewport(0, 0, 32, 32);
    for (unsigned int i = 0; i < 6; ++i) {
        const auto& [camera_target, camera_up] = camera_target_and_up[i];
        camera.lookAt(camera_target, camera_up);
        u_view.set(camera.viewMatrix());

        framebuffer.texture2D(GL_COLOR_ATTACHMENT0, irradiance, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, model.index_count, GL_UNSIGNED_INT, nullptr);
    }
    model.vao.unbind();
    framebuffer.unbind();
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
    static Model model = shape::cube(2.0f, 2.0f, 2.0f);
    return model;
}
GLProgram& Skybox::_cubemapProgram() {
    static GLProgram program = loadRenderShaderSource(cubemapVertexShader(), cubemapFragmentShader());
    return program;
}
GLProgram& Skybox::_irradianceProgram() {
    static GLProgram program = loadRenderShaderSource(cubemapVertexShader(), irradianceFragmentShader());
    return program;
}

}; // namespace toto
