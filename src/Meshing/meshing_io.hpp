#pragma once

#include <filesystem>
#include "meshing_types.hpp"

namespace Meshing {

enum class MeshExportFormat {
    PLY,
};

[[nodiscard]] auto enum_name(MeshExportFormat const& export_format) -> std::string;

void imgui_combo(MeshExportFormat& export_format);

void write_to_ply(Mesh const& mesh, std::filesystem::path const& path);

} // namespace Meshing