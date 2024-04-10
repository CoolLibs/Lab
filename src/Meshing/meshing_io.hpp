#pragma once

#include <string_view>
#include "meshing_types.hpp"

namespace Meshing {

void write_to_ply(Mesh const& mesh, std::string_view root_relative_path);

} // namespace Meshing