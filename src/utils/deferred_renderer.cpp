#include "toto-engine/utils/deferred_renderer.hpp"
#include "toto-engine/gl/gldebug.hpp"
#include "toto-engine/gl/glresources.hpp"
#include "toto-engine/import-gl.hpp"
#include "toto-engine/mesh.hpp"
#include "toto-engine/utils/camera.hpp"
#include "toto-engine/utils/shapes.hpp"
#include "toto-engine/utils/skybox.hpp"
#include <format>

namespace toto {

Model& quad() {
    static Model model = shape::quad(2.0f, 2.0f);
    return model;
}

DeferredRenderer::DeferredRenderer()
    : DeferredRenderer(800, 600) {}

DeferredRenderer::DeferredRenderer(int width, int height) {
    GLDebug::pushGroup("Create deferred renderer");

    _brdfLUT();

    _g_buffer.bind();
    _g_position.bind();
    _g_position.image2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
    _g_position.parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    _g_position.parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    _g_buffer.texture2D(GL_COLOR_ATTACHMENT0, _g_position, 0);
    _g_normal.bind();
    _g_normal.image2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
    _g_normal.parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    _g_normal.parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    _g_buffer.texture2D(GL_COLOR_ATTACHMENT1, _g_normal, 0);
    _g_albedo.bind();
    _g_albedo.image2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    _g_albedo.parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    _g_albedo.parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    _g_buffer.texture2D(GL_COLOR_ATTACHMENT2, _g_albedo, 0);
    _g_metallic_roughness_ao.bind();
    _g_metallic_roughness_ao.image2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    _g_metallic_roughness_ao.parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    _g_metallic_roughness_ao.parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    _g_buffer.texture2D(GL_COLOR_ATTACHMENT3, _g_metallic_roughness_ao, 0);
    GLuint attachments[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(4, attachments);
    _rboDepth.bind();
    _rboDepth.storage(GL_DEPTH_COMPONENT, width, height);
    _rboDepth.attach(GL_DEPTH_ATTACHMENT);
    _g_buffer.unbind();

    _uniforms_deferred["u_model"] = Uniform(_deferred, "u_model");
    _uniforms_deferred["u_view"] = Uniform(_deferred, "u_view");
    _uniforms_deferred["u_projection"] = Uniform(_deferred, "u_projection");
    _uniforms_deferred["u_albedo"] = Uniform(_deferred, "u_albedo");
    _uniforms_deferred["u_use_albedo_map"] = Uniform(_deferred, "u_use_albedo_map");
    _uniforms_deferred["u_albedo_map"] = Uniform(_deferred, "u_albedo_map");
    _uniforms_deferred["u_alpha"] = Uniform(_deferred, "u_alpha");
    _uniforms_deferred["u_use_alpha_map"] = Uniform(_deferred, "u_use_alpha_map");
    _uniforms_deferred["u_alpha_map"] = Uniform(_deferred, "u_alpha_map");
    _uniforms_deferred["u_roughness"] = Uniform(_deferred, "u_roughness");
    _uniforms_deferred["u_use_roughness_map"] = Uniform(_deferred, "u_use_roughness_map");
    _uniforms_deferred["u_roughness_map"] = Uniform(_deferred, "u_roughness_map");
    _uniforms_deferred["u_metallic"] = Uniform(_deferred, "u_metallic");
    _uniforms_deferred["u_use_metallic_map"] = Uniform(_deferred, "u_use_metallic_map");
    _uniforms_deferred["u_metallic_map"] = Uniform(_deferred, "u_metallic_map");
    _uniforms_deferred["u_ao"] = Uniform(_deferred, "u_ao");
    _uniforms_deferred["u_use_ao_map"] = Uniform(_deferred, "u_use_ao_map");
    _uniforms_deferred["u_ao_map"] = Uniform(_deferred, "u_ao_map");
    _uniforms_deferred["u_normal_map"] = Uniform(_deferred, "u_normal_map");
    _uniforms_deferred["u_use_normal_map"] = Uniform(_deferred, "u_use_normal_map");

    _uniforms_lighting["u_view"] = Uniform(_lighting, "u_view");
    _uniforms_lighting["u_position"] = Uniform(_lighting, "u_position");
    _uniforms_lighting["u_normal"] = Uniform(_lighting, "u_normal");
    _uniforms_lighting["u_albedo"] = Uniform(_lighting, "u_albedo");
    _uniforms_lighting["u_metallic_roughness_ao"] = Uniform(_lighting, "u_metallic_roughness_ao");
    _uniforms_lighting["u_light_direction"] = Uniform(_lighting, "u_light_direction");
    _uniforms_lighting["u_light_intensity"] = Uniform(_lighting, "u_light_intensity");
    _uniforms_lighting["u_light_color"] = Uniform(_lighting, "u_light_color");
    _uniforms_lighting["u_irradiance_map"] = Uniform(_lighting, "u_irradiance_map");
    _uniforms_lighting["u_prefiltered_map"] = Uniform(_lighting, "u_prefiltered_map");
    _uniforms_lighting["u_brdf_lut"] = Uniform(_lighting, "u_brdf_lut");
    _uniforms_lighting["u_exposure"] = Uniform(_lighting, "u_exposure");

    GLDebug::popGroup();
}

void DeferredRenderer::useDeferredProgram() {
    _deferred.use();
}
void DeferredRenderer::useLightingProgram() {
    _lighting.use();
}

void DeferredRenderer::enableDepthTest() {
    glEnable(GL_DEPTH_TEST);
}
void DeferredRenderer::disableDepthTest() {
    glDisable(GL_DEPTH_TEST);
}
void DeferredRenderer::clear(bool color, bool depth) {
    glClear((color ? GL_COLOR_BUFFER_BIT : 0) | (depth ? GL_DEPTH_BUFFER_BIT : 0));
}
void DeferredRenderer::setViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
    _g_buffer.bind();
    _g_position.bind();
    _g_position.image2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
    _g_normal.bind();
    _g_normal.image2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
    _g_albedo.bind();
    _g_albedo.image2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    _g_metallic_roughness_ao.bind();
    _g_metallic_roughness_ao.image2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    _rboDepth.bind();
    _rboDepth.storage(GL_DEPTH_COMPONENT, width, height);
    glViewport(0, 0, width, height);
    _g_buffer.unbind();
}
void DeferredRenderer::bindGBuffer() {
    _g_buffer.bind();
}
void DeferredRenderer::unbindGBuffer() {
    _g_buffer.unbind();
}

void DeferredRenderer::setTransform(const Transform& transform) {
    useDeferredProgram();
    _uniforms_deferred["u_model"].set(transform.matrix());
}

void DeferredRenderer::setCamera(const Camera& camera) {
    useDeferredProgram();
    _uniforms_deferred["u_view"].set(camera.viewMatrix());
    _uniforms_deferred["u_projection"].set(camera.projectionMatrix());
    useLightingProgram();
    _uniforms_lighting["u_view"].set(camera.viewMatrix());
}
void DeferredRenderer::setSkybox(const Skybox& skybox) {
    // _uniforms_lighting["u_irradiance_map"].set(skybox.irradiance(), 4);
    _skybox = &skybox;
}

void DeferredRenderer::setMaterial(const Material& material) {
    _uniforms_deferred["u_albedo"].set(material.albedo);
    _setMap("albedo", material.albedo_map, 1);
    _uniforms_deferred["u_alpha"].set(material.alpha);
    _setMap("alpha", material.alpha_map, 2);
    _uniforms_deferred["u_roughness"].set(material.roughness);
    _setMap("roughness", material.roughness_map, 3);
    _uniforms_deferred["u_metallic"].set(material.metallic);
    _setMap("metallic", material.metallic_map, 4);
    _uniforms_deferred["u_ao"].set(material.ao);
    _setMap("ao", material.ao_map, 5);
    _setMap("normal", material.normal_map, 6);
}
void DeferredRenderer::setLight(const Light& light) {
    _uniforms_lighting["u_light_direction"].set(light.direction());
    _uniforms_lighting["u_light_intensity"].set(light.intensity());
    _uniforms_lighting["u_light_color"].set(light.color());
}

void DeferredRenderer::setExposure(float exposure) {
    useLightingProgram();
    _uniforms_lighting["u_exposure"].set(exposure);
}

void DeferredRenderer::draw(const Model& model) {
    model.vao.bind();
    glDrawElements(GL_TRIANGLES, model.index_count, GL_UNSIGNED_INT, nullptr);
    model.vao.unbind();
}

void DeferredRenderer::draw(const Model& model, const Material& material, const Transform& transform) {
    GLDebug::pushGroup(std::format("Draw model {} with material {}", model.name, material.name));
    setTransform(transform);
    setMaterial(material);
    draw(model);
    GLDebug::popGroup();
}

void DeferredRenderer::beginRender() {
    GLDebug::pushGroup("Deferred rendering");
    GLDebug::pushGroup("Prepare frame");

    bindGBuffer();
    _deferred.use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLDebug::popGroup();
}

void DeferredRenderer::endRender(const std::optional<GLFrameBuffer<>>& framebuffer) {
    GLDebug::pushGroup("Render frame");

    if (framebuffer.has_value()) {
        framebuffer->bind();
    } else {
        GLFrameBuffer<>::unbind();
    }

    auto& lut = _brdfLUT();

    _lighting.use();
    _uniforms_lighting["u_position"].set(_g_position, 0);
    _uniforms_lighting["u_normal"].set(_g_normal, 1);
    _uniforms_lighting["u_albedo"].set(_g_albedo, 2);
    _uniforms_lighting["u_metallic_roughness_ao"].set(_g_metallic_roughness_ao, 3);
    _uniforms_lighting["u_irradiance_map"].set(_skybox->irradiance(), 4);
    _uniforms_lighting["u_prefiltered_map"].set(_skybox->prefiltered(), 5);
    _uniforms_lighting["u_brdf_lut"].set(lut, 6);
    draw(quad());
    _g_position.unbind();
    _g_normal.unbind();
    _g_albedo.unbind();
    _g_metallic_roughness_ao.unbind();

    GLDebug::popGroup();
    GLDebug::popGroup();
}

void DeferredRenderer::_setMap(
    const std::string& name, const std::optional<std::reference_wrapper<GLTexture2D>>& map, uint index
) {
    const auto use_name = "u_use_" + name + "_map";
    const auto map_name = "u_" + name + "_map";

    _uniforms_deferred[use_name].set(map.has_value());
    if (map.has_value()) {
        map->get().bind();
        _uniforms_deferred[map_name].set(map->get(), index);
    }
}

GLTexture2D& DeferredRenderer::_brdfLUT() {
    static GLTexture2D lut = [&] {
        GLDebug::pushGroup("Creating BRDF LUT");

        GLTexture2D texture;
        texture.bind();
        texture.image2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, nullptr);
        texture.parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        texture.parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        texture.parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        texture.parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        auto framebuffer = GLFrameBuffer<>();
        auto renderbuffer = GLRenderBuffer<>();
        auto program = loadRenderShaderSource(screenMaterialVertexShader(), brdfLUTFragmentShader());

        framebuffer.bind();
        renderbuffer.storage(GL_DEPTH_COMPONENT24, 512, 512);
        framebuffer.texture2D(GL_COLOR_ATTACHMENT0, texture, 0);

        glViewport(0, 0, 512, 512);
        program.use();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        quad().vao.bind();
        glDrawElements(GL_TRIANGLES, quad().index_count, GL_UNSIGNED_INT, nullptr);
        quad().vao.unbind();
        GLFrameBuffer<>::unbind();

        GLDebug::popGroup();

        return texture;
    }();

    return lut;
}

} // namespace toto
