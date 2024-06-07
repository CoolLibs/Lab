#pragma once
#include "Module/ShaderBased/DataToGenerateShaderCode.hpp"

namespace Lab {

auto generate_simulation_shader_code(
    Cool::NodeId const& root_node_id,
    Cool::NodeId&       id_of_node_storing_particles_count,
    int                 dimension,
    DataToGenerateShaderCode const&
) -> tl::expected<std::string, std::string>;

} // namespace Lab
