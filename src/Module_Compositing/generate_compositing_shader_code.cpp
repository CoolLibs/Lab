#include "generate_compositing_shader_code.h"
#include "Cool/String/String.h"
#include "Nodes/CodeGen.h"

namespace Lab {

static auto gen_all_output_indices_declarations(Cool::NodesGraph const& graph)
    -> std::string
{
    std::stringstream res{};

    graph.for_each_node<Node>([&](Node const& node) {
        for (size_t i = 1; i < node.output_pins().size(); ++i)
            res << fmt::format("float {};\n", make_valid_output_index_name(node.output_pins()[i]));
    });

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

auto generate_compositing_shader_code(
    Cool::NodesGraph const&                     graph,
    Cool::NodeId const&                         root_node_id,
    Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition,
    Cool::InputProvider_Ref                     input_provider,
    NodeDefinitionCallback const&               node_definition_callback
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
        root_node_id,
        context,
        node_definition_callback
    );
    if (!main_function_name)
        return tl::make_unexpected(fmt::format("Failed to generate shader code:\n{}", main_function_name.error()));

    using fmt::literals::operator""_a;
    return fmt::format(
        FMT_COMPILE(R"glsl(#version 410

uniform float     _time;
uniform float     _height;
uniform float     _audio_volume;
uniform sampler1D _audio_spectrum;
uniform sampler1D _audio_waveform;
uniform mat3      _camera2D;
uniform mat3      _camera2D_inverse;
uniform sampler2D _previous_frame_texture;
uniform sampler2D _particles_texture;
out vec4          out_Color;

#include "_ROOT_FOLDER_/res/shader-utils.glsl"
#include "_COOL_RES_/shaders/math.glsl"
#include "_COOL_RES_/shaders/color_conversions.glsl"
#include "_COOL_RES_/shaders/Texture.glsl"
#include "_COOL_RES_/shaders/camera.glsl"

uniform sampler2D mixbox_lut; // The uniform must have this exact name that mixbox.glsl expects.
#include "_ROOT_FOLDER_/res/mixbox/mixbox.glsl"

vec2 to_view_space(vec2 uv)
{{
    vec3 p = _camera2D_inverse * vec3(uv, 1.);
    return p.xy / p.z;
}}

struct CoollabContext
{{
    vec2 uv;
}};

{output_indices_declarations}

{main_function_implementation}

void main()
{{
    vec2 uv = normalized_uv();
    vec3 tmp = _camera2D * vec3(uv, 1.);
    uv = tmp.xy / tmp.z;
    CoollabContext coollab_context;
    coollab_context.uv = uv;
    out_Color = {main_function_name}(coollab_context, uv);
}}

)glsl"
        ),
        "output_indices_declarations"_a  = gen_all_output_indices_declarations(graph),
        "main_function_implementation"_a = inject_context_argument_in_all_functions(context.code(), context.function_names()),
        "main_function_name"_a           = *main_function_name
    );
}

} // namespace Lab