#include "toto-engine/utils/skybox.hpp"
#include "toto-engine/gl/glresources.hpp"
#include "toto-engine/import-gl.hpp"
#include "toto-engine/loader/shader.hpp"
#include "toto-engine/uniform.hpp"
#include "toto-engine/utils/shapes.hpp"
#include <GL/gl.h>

namespace toto {

Skybox::Skybox() {
    _generateCubemap();
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

Skybox::Skybox(const GLTexture2D& hdri_texture)
    : Skybox() {
    _renderToCubemap(hdri_texture);
    auto& program = _cubemapProgram();
    auto& model = _cubemapModel();
    auto& texture = _cubemap;
    auto& framebuffer = _framebuffer;
    auto& renderbuffer = _renderbuffer;

    framebuffer.bind();
    renderbuffer.storage(GL_DEPTH_COMPONENT24, 512, 512);
    renderbuffer.attach(GL_DEPTH_ATTACHMENT);
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

void Skybox::_generateCubemap() {
    auto& cubemap = _cubemap;
    cubemap.parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    cubemap.parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    cubemap.parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    cubemap.parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    cubemap.parameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    for (int i = 0; i < 6; i++) {
        cubemap.image2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    }
}
void Skybox::_renderToCubemap(const GLTexture2D& hdri_texture) {
    auto& program = _cubemapProgram();
    auto& model = _cubemapModel();
    auto& cubemap = _cubemap;
    auto& framebuffer = _framebuffer;
    auto& renderbuffer = _renderbuffer;

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
    glViewport(0, 0, 512, 512);
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

GLProgram& Skybox::_cubemapProgram() {
    static GLProgram program = loadRenderShaderSource(cubemapVertexShader(), cubemapFragmentShader());
    return program;
}
Model& Skybox::_cubemapModel() {
    static Model model = shape::cube(2.0f, 2.0f, 2.0f);
    return model;
}

}; // namespace toto
