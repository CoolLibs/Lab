#pragma once
#include "Cool/Mesh/Mesh.hpp"
#include "Cool/Nodes/NodeId.h"
#include "MeshingSettings.hpp"
#include "Module/ShaderBased/DataToGenerateShaderCode.hpp"
#include "Module/ShaderBased/DataToPassToShader.hpp"

namespace Lab {

// TODO(Meshing) Return error message in case of failure (tl::expected)
auto gen_mesh_from_sdf(Cool::NodeId const& main_node_id, MeshingSettings const&, DataToPassToShader const&, DataToGenerateShaderCode const&) -> std::optional<Cool::Mesh>;

} // namespace Lab