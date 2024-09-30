#include "toto-engine/loader/shader.hpp"
#include "toto-engine/gl/glresources.hpp"
#include <fstream>

namespace toto {

template <GLShaderType Type>
GLShader<Type> loadShaderSource(const std::string& source) {
    GLShader<Type> shader;
    shader.source(source);
    shader.compile();
    return shader;
}

GLProgram loadRenderShaderSource(const std::string& vertex_shader_source, const std::string& fragment_shader_source) {
    auto vertex_shader = loadShaderSource<GLShaderType::Vertex>(vertex_shader_source);
    auto fragment_shader = loadShaderSource<GLShaderType::Fragment>(fragment_shader_source);
    GLProgram program;
    program.attachShaders(vertex_shader, fragment_shader);
    program.link();
    return program;
}

GLProgram loadComputeShaderSource(const std::string& compute_shader_source) {
    auto compute_shader = loadShaderSource<GLShaderType::Compute>(compute_shader_source);
    GLProgram program;
    program.attachShaders(compute_shader);
    program.link();
    return program;
}

template <GLShaderType Type>
GLShader<Type> loadShader(const std::filesystem::path& path) {
    std::ifstream file(path);
    std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return loadShaderSource<Type>(source);
}

GLProgram loadRenderShaderFile(
    const std::filesystem::path& vertex_shader_path, const std::filesystem::path& fragment_shader_path
) {
    auto vertex_shader = loadShader<GLShaderType::Vertex>(vertex_shader_path);
    auto fragment_shader = loadShader<GLShaderType::Fragment>(fragment_shader_path);
    GLProgram program;
    program.attachShaders(vertex_shader, fragment_shader);
    program.link();
    return program;
}

GLProgram loadComputeShaderFile(const std::filesystem::path& compute_shader_path) {
    auto compute_shader = loadShader<GLShaderType::Compute>(compute_shader_path);
    GLProgram program;
    program.attachShaders(compute_shader);
    program.link();
    return program;
}

} // namespace toto