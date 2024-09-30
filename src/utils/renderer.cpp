#include "toto-engine/utils/renderer.hpp"
#include "toto-engine/mesh.hpp"

namespace toto {

Renderer::Renderer() {
    _program.use();
    _uniforms["u_model"] = Uniform(_program, "u_model");
    _uniforms["u_view"] = Uniform(_program, "u_view");
    _uniforms["u_projection"] = Uniform(_program, "u_projection");

    _uniforms["u_albedo"] = Uniform(_program, "u_albedo");
    _uniforms["u_use_albedo_map"] = Uniform(_program, "u_use_albedo_map");
    _uniforms["u_albedo_map"] = Uniform(_program, "u_albedo_map");

    _uniforms["u_alpha"] = Uniform(_program, "u_alpha");
    _uniforms["u_use_alpha_map"] = Uniform(_program, "u_use_alpha_map");
    _uniforms["u_alpha_map"] = Uniform(_program, "u_alpha_map");

    _uniforms["u_roughness"] = Uniform(_program, "u_roughness");
    _uniforms["u_use_roughness_map"] = Uniform(_program, "u_use_roughness_map");
    _uniforms["u_roughness_map"] = Uniform(_program, "u_roughness_map");

    _uniforms["u_metallic"] = Uniform(_program, "u_metallic");
    _uniforms["u_use_metallic_map"] = Uniform(_program, "u_use_metallic_map");
    _uniforms["u_metallic_map"] = Uniform(_program, "u_metallic_map");

    _uniforms["u_ao"] = Uniform(_program, "u_ao");
    _uniforms["u_use_ao_map"] = Uniform(_program, "u_use_ao_map");
    _uniforms["u_ao_map"] = Uniform(_program, "u_ao_map");

    _uniforms["u_normal_map"] = Uniform(_program, "u_normal_map");
    _uniforms["u_use_normal_map"] = Uniform(_program, "u_use_normal_map");

    _uniforms["u_light_direction"] = Uniform(_program, "u_light_direction");
    _uniforms["u_light_intensity"] = Uniform(_program, "u_light_intensity");
    _uniforms["u_light_color"] = Uniform(_program, "u_light_color");
}

void Renderer::useProgram() {
    _program.use();
}
void Renderer::enableDepthTest() {
    glEnable(GL_DEPTH_TEST);
}
void Renderer::disableDepthTest() {
    glDisable(GL_DEPTH_TEST);
}
void Renderer::clear(bool color, bool depth) {
    glClear((color ? GL_COLOR_BUFFER_BIT : 0) | (depth ? GL_DEPTH_BUFFER_BIT : 0));
}
void Renderer::setViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void Renderer::setModelMatrix(const glm::mat4& model_matrix) {
    _uniforms["u_model"].set(model_matrix);
}
void Renderer::setViewMatrix(const glm::mat4& view_matrix) {
    _uniforms["u_view"].set(view_matrix);
}
void Renderer::setProjectionMatrix(const glm::mat4& projection_matrix) {
    _uniforms["u_projection"].set(projection_matrix);
}

void Renderer::setTransform(const Transform& transform) {
    setModelMatrix(transform.matrix());
}

void Renderer::setCamera(const Camera& camera) {
    setViewMatrix(camera.viewMatrix());
    setProjectionMatrix(camera.projectionMatrix());
}

void Renderer::setMaterial(const Material& material) {
    _uniforms["u_albedo"].set(material.albedo);
    _setMap("albedo", material.albedo_map, 1);
    _uniforms["u_alpha"].set(material.alpha);
    _setMap("alpha", material.alpha_map, 2);
    _uniforms["u_roughness"].set(material.roughness);
    _setMap("roughness", material.roughness_map, 3);
    _uniforms["u_metallic"].set(material.metallic);
    _setMap("metallic", material.metallic_map, 4);
    _uniforms["u_ao"].set(material.ao);
    _setMap("ao", material.ao_map, 5);
    _setMap("normal", material.normal_map, 6);
}

void Renderer::setLight(const Light& light) {
    if (!light.isDirectional()) {
        throw std::runtime_error("Only directional lights are supported for now");
    }
    _uniforms["u_light_direction"].set(light.direction());
    _uniforms["u_light_intensity"].set(light.intensity());
    _uniforms["u_light_color"].set(light.color());
}

void Renderer::render(const Model& model) {
    model.vao.bind();
    glDrawElements(GL_TRIANGLES, model.index_count, GL_UNSIGNED_INT, nullptr);
}

void Renderer::render(const Model& model, const Material& material, const Transform& transform) {
    useProgram();
    setTransform(transform);
    setMaterial(material);
    render(model);
}

void Renderer::_setMap(
    const std::string& name, const std::optional<std::reference_wrapper<GLTexture2D>>& map, uint index
) {
    const auto use_name = "u_use_" + name + "_map";
    const auto map_name = "u_" + name + "_map";

    _uniforms[use_name].set(map.has_value());
    if (map.has_value()) {
        map->get().bind();
        _uniforms[map_name].set(map->get(), index);
    }
}

} // namespace toto
