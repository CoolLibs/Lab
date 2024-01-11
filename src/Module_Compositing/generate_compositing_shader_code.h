#pragma once
#include <Cool/Nodes/GetNodeDefinition_Ref.h>
#include <Cool/Nodes/NodesGraph.h>
#include "Cool/Dependencies/InputProvider_Ref.h"
#include "Nodes/MaybeGenerateModule.h"
#include "Nodes/NodeDefinition.h"

namespace Lab {

auto generate_compositing_shader_code(
    Cool::NodesGraph const&,
    Cool::NodeId const& root_node_id,
    Cool::GetNodeDefinition_Ref<NodeDefinition>,
    Cool::InputProvider_Ref,
    MaybeGenerateModule const&                       maybe_generate_module,
    std::function<std::vector<std::string>()> const& get_textures_names
) -> tl::expected<std::string, std::string>;

} // namespace Lab