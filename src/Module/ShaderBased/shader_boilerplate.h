#pragma once
#include <Cool/Nodes/GetNodeDefinition_Ref.h>
#include <Cool/Nodes/NodesGraph.h>
#include <Nodes/CodeGenContext.h>
#include <functional>
#include <string>
#include "Cool/Dependencies/InputProvider_Ref.h"
#include "Nodes/CodeGen.h"
#include "Nodes/MaybeGenerateModule.h"
#include "Nodes/NodeDefinition.h"

namespace Lab {

using MakeShaderMain = std::function<std::string(std::string const& main_function_name)>;

struct ShaderContent {
    std::string    version;
    std::string    before_main;
    MakeShaderMain make_main;
};

auto generate_shader_code(
    Cool::NodesGraph const&,
    Cool::NodeId const& root_node_id,
    Cool::GetNodeDefinition_Ref<NodeDefinition>,
    Cool::InputProvider_Ref,
    MaybeGenerateModule const&,
    FunctionSignature const&,
    ShaderContent const&,
    std::function<std::vector<std::string>()> const& get_textures_names
) -> tl::expected<std::string, std::string>;

} // namespace Lab