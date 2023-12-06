#include "generate_simulation_shader_code.h"
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

auto generate_simulation_shader_code(
    Cool::NodesGraph const&                     graph,
    Cool::NodeId const&                         root_node_id,
    Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition,
    Cool::InputProvider_Ref                     input_provider
)
    -> tl::expected<std::string, std::string>
{
    auto       context            = CodeGenContext{graph, get_node_definition, input_provider};
    auto const main_function_name = gen_desired_function(
        FunctionSignature{
            .from  = PrimitiveType::Particle,
            .to    = PrimitiveType::Particle, // We output sRGB and straight alpha because this is what the rest of the world expects most of the time.
            .arity = 1,
        },
        root_node_id,
        context
    );
    if (!main_function_name)
        return tl::make_unexpected(fmt::format("Failed to generate shader code:\n{}", main_function_name.error()));

    using fmt::literals::operator""_a;
    return fmt::format(
        FMT_COMPILE(R"glsl(
uniform float _delta_time;
uniform float     _time;
uniform float     _height;
uniform float     _audio_volume;
uniform sampler1D _audio_spectrum;
uniform sampler1D _audio_waveform;
uniform mat3      _camera2D;
uniform mat3      _camera2D_inverse;
uniform sampler2D _previous_frame_texture;
uniform sampler2D _particles_texture;

#include "_ROOT_FOLDER_/res/shader-utils.glsl"
#include "_COOL_RES_/shaders/math.glsl"
#include "_COOL_RES_/shaders/color_conversions.glsl"
#include "_COOL_RES_/shaders/Texture.glsl"
#include "_COOL_RES_/shaders/camera.glsl"

layout(std430, binding = 0) buffer _positions_buffer
{{
    float _positions[];
}};

layout(std430, binding = 1) buffer _velocities_buffer
{{
    float _velocities[];
}};

struct Particle
{{
    vec2 position;
    vec2 velocity;
    vec2 acceleration;
}};

struct CoollabContext
{{
    vec2 uv;
}};

{output_indices_declarations}

{main_function_implementation}

void cool_main()
{{
    uint     gid = gl_GlobalInvocationID.x;
    Particle particle;
    particle.position     = vec2(_positions[gid * 2], _positions[gid * 2 + 1]);
    particle.velocity     = vec2(_velocities[gid * 2], _velocities[gid * 2 + 1]);
    particle.acceleration = vec2(0.);

    CoollabContext coollab_context;
    coollab_context.uv = vec2(0.);

    particle = {main_function_name}(coollab_context, particle);

    particle.velocity += particle.acceleration * _delta_time;
    particle.position += particle.velocity * _delta_time; // TODO(Particles) proper integration method

    _positions[gid * 2]      = particle.position.x;
    _positions[gid * 2 + 1]  = particle.position.y;
    _velocities[gid * 2]     = particle.velocity.x;
    _velocities[gid * 2 + 1] = particle.velocity.y;
}}
)glsl"
        ),
        "output_indices_declarations"_a  = gen_all_output_indices_declarations(graph),
        "main_function_implementation"_a = inject_context_argument_in_all_functions(context.code(), context.function_names()),
        "main_function_name"_a           = *main_function_name
    );
}

} // namespace Lab