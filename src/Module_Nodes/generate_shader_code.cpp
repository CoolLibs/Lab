#include "generate_shader_code.h"
#include "CodeGen.h"

namespace Lab {

static auto gen_all_output_indices_declarations(Graph const& graph)
    -> std::string
{
    std::stringstream res{};

    std::shared_lock lock{graph.nodes().mutex()};
    for (auto const& [_, node] : graph.nodes())
        for (size_t i = 1; i < node.output_pins().size(); ++i)
            res << fmt::format("float {};\n", make_valid_output_index_name(node.output_pins()[i]));

    return res.str();
}

auto generate_shader_code(
    Cool::NodeId const&                                main_node_id,
    Graph const&                                       graph,
    Cool::GetNodeDefinition_Ref<NodeDefinition> const& get_node_definition,
    Cool::InputProvider_Ref                            input_provider
)
    -> tl::expected<std::string, std::string>
{
    auto       context            = CodeGenContext{graph, get_node_definition, input_provider};
    auto const main_function_name = gen_desired_function(
        Signature::ImageRGBA,
        main_node_id,
        context
    );
    if (!main_function_name)
        return tl::make_unexpected(fmt::format("Failed to generate shader code:\n{}", main_function_name.error()));

    using fmt::literals::operator""_a;
    return fmt::format(
        FMT_COMPILE(R"STR(#version 410

uniform float _time;
out vec4      out_Color;

#include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
#include "_COOL_RES_/shaders/math.glsl"

{output_indices_declarations}

{main_function_implementation}

void main()
{{
    vec2 uv = normalized_uv();
    out_Color = vec4({main_function_name}(uv));
    out_Color.rgb /= out_Color.a; // Output straight alpha
}}

)STR"
        ),
        "output_indices_declarations"_a  = gen_all_output_indices_declarations(graph),
        "main_function_implementation"_a = context.code(),
        "main_function_name"_a           = *main_function_name
    );
}

} // namespace Lab