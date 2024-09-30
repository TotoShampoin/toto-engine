#pragma once

#include "toto-engine/mesh.hpp"
#include <filesystem>
#include <tuple>

namespace toto {

/**
 * @warning NOT IMPLEMENTED
 */
std::vector<std::tuple<Model, Material>> loadWavefront(const std::filesystem::path& path);

} // namespace toto
