#pragma once

#include <string_view>
#include "meshing_types.hpp"

namespace Meshing {

void write_to_ply(Mesh const& mesh, std::filesystem::path const& path);

} // namespace Meshing