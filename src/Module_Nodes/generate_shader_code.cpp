#include "generate_shader_code.h"
#include <string>
#include "CodeGen.h"
#include "Cool/String/String.h"
#include "Module_Nodes/PrimitiveType.h"

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

static auto inject_context_argument_in_all_functions(std::string code, std::vector<std::string> const& function_names)
    -> std::string
{
    for (auto const& name : function_names)
    {
        Cool::String::replace_all(code, name + "(", name + "(coollab_context, ");
        Cool::String::replace_all(code, name + "/*coollabdef*/(", name + "(CoollabContext coollab_context, ");
    }

    // Fixup the extra commas for functions that had no arguments initially
    Cool::String::replace_all(code, ", )", ")");
    Cool::String::replace_all(code, "(coollab_context, ()", "(coollab_context");

    return code;
}

auto generate_shader_code(
    Cool::NodeId const&                         main_node_id,
    Graph const&                                graph,
    Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition,
    Cool::InputProvider_Ref                     input_provider
)
    -> tl::expected<std::string, std::string>
{
    auto       context            = CodeGenContext{graph, get_node_definition, input_provider};
    auto const main_function_name = gen_desired_function(
        FunctionSignature{
            .from  = PrimitiveType::UV,
            .to    = PrimitiveType::sRGB_StraightA, // We output sRGB and straight alpha because this is what the rest of the world expects most of the time.
            .arity = 1,
        },
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

#include "_ROOT_FOLDER_/res/shader-utils/normalized_uv.glsl"
#include "_COOL_RES_/shaders/math.glsl"
#include "_COOL_RES_/shaders/color_conversions.glsl"

struct CoollabContext
{{
    vec2 uv;
}};

float coollab_global_alpha = 1.;

{output_indices_declarations}

{main_function_implementation}

void main()
{{
    vec2 uv = normalized_uv();
    CoollabContext coollab_context;
    coollab_context.uv = uv;
    out_Color = {main_function_name}(coollab_context, uv);
}}

)STR"
        ),
        "output_indices_declarations"_a  = gen_all_output_indices_declarations(graph),
        "main_function_implementation"_a = inject_context_argument_in_all_functions(context.code(), context.function_names()),
        "main_function_name"_a           = *main_function_name
    );
}

} // namespace Lab