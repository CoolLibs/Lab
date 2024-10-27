#include "generate_simulation_shader_code.h"
#include <Nodes/PrimitiveType.h>
#include "Cool/File/File.h"
#include "Cool/Path/Path.h"
#include "Module/ShaderBased/generate_shader_code.hpp"
#include "Nodes/Node.h"
#include "simulation_code_gen_helpers.h"

namespace Lab {

auto generate_simulation_shader_code(
    Cool::NodeId const&             root_node_id,
    Cool::NodeId&                   id_of_node_storing_particles_count,
    int                             dimension,
    DataToGenerateShaderCode const& data,
    MaybeGenerateModule const&      maybe_generate_module
) -> tl::expected<std::string, std::string>
{
    return Cool::File::to_string(Cool::Path::root() / "res/Particles/sim.comp");
}

} // namespace Lab
