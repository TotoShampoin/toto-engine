#pragma once

#include "toto-engine/gl/glresources.hpp"
#include <filesystem>

namespace toto {

GLProgram loadRenderShaderSource(const std::string& vertex_shader_source, const std::string& fragment_shader_source);

GLProgram loadComputeShaderSource(const std::string& compute_shader_source);

GLProgram loadRenderShaderFile(
    const std::filesystem::path& vertex_shader_path, const std::filesystem::path& fragment_shader_path
);

GLProgram loadComputeShaderFile(const std::filesystem::path& compute_shader_path);

} // namespace toto
