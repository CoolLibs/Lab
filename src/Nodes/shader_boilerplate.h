#pragma once
#include <Cool/Nodes/GetNodeDefinition_Ref.h>
#include <Cool/Nodes/NodesGraph.h>
#include <Nodes/CodeGenContext.h>
#include <functional>
#include <string>
#include "CodeGen.h"
#include "Cool/Dependencies/InputProvider_Ref.h"
#include "Nodes/MaybeGenerateModule.h"
#include "Nodes/NodeDefinition.h"


namespace Lab {

using ShaderMainMaker = std::function<
    std::string(
        std::string const& main_function_name
    )>;

struct ShaderContent {
    std::string     version;
    std::string     uniforms;
    std::string     includes;
    std::string     structuration;
    ShaderMainMaker main;
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

auto valid_property_name(
    std::string const& name,
    reg::AnyId const&  property_default_variable_id
) -> std::string;

} // namespace Lab