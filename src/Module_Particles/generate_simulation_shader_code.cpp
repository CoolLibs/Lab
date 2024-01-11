#include "generate_simulation_shader_code.h"
#include <Nodes/PrimitiveType.h>
#include <optional>
#include "Module/ShaderBased/shader_boilerplate.h"
#include "fmt/compile.h"
#include "microcode_generator.h"

namespace Lab {

auto generate_simulation_shader_code(
    Cool::NodesGraph const&                     graph,
    Cool::NodeId const&                         root_node_id,
    Cool::NodeId&                               initializer_node_id,
    Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition,
    Cool::InputProvider_Ref                     input_provider,
    int                                         dimension
) -> tl::expected<std::string, std::string>
{
    using fmt::literals::operator""_a;
    auto const content = ShaderContent{
        .version     = "",
        .before_main = fmt::format(FMT_COMPILE(R"glsl(
layout(std430, binding = 0) buffer _positions_buffer
{{
    float _positions[];
}};
layout(std430, binding = 1) buffer _velocities_buffer
{{
    float _velocities[];
}};
layout(std430, binding = 2) buffer _sizes_buffer
{{
    float _sizes[];
}};
layout(std430, binding = 3) buffer _lifetimes_buffer
{{
    float _lifetimes[];
}};
layout(std430, binding = 4) buffer _lifetime_maxs_buffer
{{
    float _lifetime_maxs[];
}};
layout(std430, binding = 5) buffer _colors_buffer
{{
    float _colors[];
}};

struct Particle{N}D
{{
    vec{N} position;
    vec{N} velocity;
    vec{N} acceleration;
    float  size;
    float  lifetime;
    float  lifetime_max;
    vec4   color;
    uint   id;
}};

struct CoollabContext
{{
    vec2 uv;
    Particle{N}D particle;
}};

)glsl"),
                                   "N"_a = dimension),
        .make_main   = [&](std::string const& main_function_name) {
            return fmt::format(
                R"glsl(
void cool_main()
{{
    uint     gid = gl_GlobalInvocationID.x;
    Particle{N}D particle;
    particle.position     = {position_b2v};
    particle.velocity     = {velocity_b2v};
    particle.acceleration = {acceler_zero};
    particle.size         = _sizes[gid];
    particle.lifetime     = _lifetimes[gid];
    particle.lifetime_max = _lifetime_maxs[gid];
    particle.color.x      = _colors[gid * 4];
    particle.color.y      = _colors[gid * 4 + 1];
    particle.color.z      = _colors[gid * 4 + 2];
    particle.color.w      = _colors[gid * 4 + 3];
    particle.id           = gid;

    CoollabContext coollab_context;
    coollab_context.uv = particle.position.xy;
    coollab_context.particle = particle;

    particle = {main_function_name}(coollab_context, particle);

    particle.velocity += particle.acceleration * _delta_time;
    particle.position += particle.velocity * _delta_time; // TODO(Particles) proper integration method

    {position_v2b}
    {velocity_v2b}
    _sizes[gid]              = particle.size;
    _lifetimes[gid]          = particle.lifetime;
    _lifetime_maxs[gid]      = particle.lifetime_max;
    _colors[gid * 4]         = particle.color.x;
    _colors[gid * 4 + 1]     = particle.color.y;
    _colors[gid * 4 + 2]     = particle.color.z;
    _colors[gid * 4 + 3]     = particle.color.w;
}})glsl",
                "main_function_name"_a = main_function_name,
                "position_b2v"_a       = buffer_to_vec(dimension, "_positions", "gid"),
                "velocity_b2v"_a       = buffer_to_vec(dimension, "_velocities", "gid"),
                "position_v2b"_a       = vec_to_buffer(dimension, "_positions", "gid", "particle.position"),
                "velocity_v2b"_a       = vec_to_buffer(dimension, "_velocities", "gid", "particle.velocity"),
                "acceler_zero"_a       = vec_zero(dimension),
                "N"_a                  = dimension
            );
        },
    };

    auto const main_function_signature = FunctionSignature{
        .from  = primitive_type_particle(dimension),
        .to    = primitive_type_particle(dimension),
        .arity = 1,
    };

    auto const node_definition_callback = [&graph, &initializer_node_id](auto const& node_id, auto const&) {
        auto maybe_node = graph.try_get_node<Node>(node_id);
        if (maybe_node != nullptr && maybe_node->is_particle_initializer())
        {
            initializer_node_id = node_id;
        }
        return std::nullopt;
    };

    return generate_shader_code(
        graph,
        root_node_id,
        get_node_definition,
        input_provider,
        node_definition_callback,
        main_function_signature,
        content,
        []() { return std::vector<std::string>(); }
    );
}

} // namespace Lab
