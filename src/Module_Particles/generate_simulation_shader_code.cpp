#include "generate_simulation_shader_code.h"
#include <optional>
#include "Nodes/shader_boilerplate.h"

// #include "Cool/String/String.h"
// #include "Nodes/CodeGen.h"

namespace Lab {

auto generate_simulation_shader_code(
    Cool::NodesGraph const&                     graph,
    Cool::NodeId const&                         root_node_id,
    Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition,
    Cool::InputProvider_Ref                     input_provider
)
    -> tl::expected<std::string, std::string>
{
    using fmt::literals::operator""_a;
    ShaderContent content{
        .version       = "",
        .uniforms      = R"glsl()glsl",
        .includes      = R"glsl()glsl",
        .structuration = R"glsl(
layout(std430, binding = 0) buffer _positions_buffer
{
    float _positions[];
};

layout(std430, binding = 1) buffer _velocities_buffer
{
    float _velocities[];
};

struct Particle
{
    vec3 position;
    vec3 velocity;
    vec3 acceleration;
};
)glsl",
        .main          = [](
                    std::string const& main_function_name
                ) -> std::string {
            return fmt::format(
                FMT_COMPILE(R"glsl(
void cool_main()
{{
    uint     gid = gl_GlobalInvocationID.x;
    Particle particle;
    particle.position     = vec3(_positions [gid * 3], _positions [gid * 3 + 1], _positions [gid * 3 + 2]);
    particle.velocity     = vec3(_velocities[gid * 3], _velocities[gid * 3 + 1], _velocities[gid * 3 + 2]);
    particle.acceleration = vec3(0.);

    CoollabContext coollab_context;
    coollab_context.uv = particle.position.xy;

    particle = {main_function_name}(coollab_context, particle);

    particle.velocity += particle.acceleration * _delta_time;
    particle.position += particle.velocity * _delta_time; // TODO(Particles) proper integration method

    _positions [gid * 3]     = particle.position.x;
    _positions [gid * 3 + 1] = particle.position.y;
    _positions [gid * 3 + 2] = particle.position.z;
    _velocities[gid * 3]     = particle.velocity.x;
    _velocities[gid * 3 + 1] = particle.velocity.y;
    _velocities[gid * 3 + 2] = particle.velocity.z;
}}
)glsl"),
                "main_function_name"_a = main_function_name
            );
        },
    };

    FunctionSignature signature{
        .from  = PrimitiveType::Particle,
        .to    = PrimitiveType::Particle,
        .arity = 1,
    };

    auto node_definition_callback = [](auto const&, auto const&) {
        return std::nullopt;
    };

    return generate_shader_code(
        graph,
        root_node_id,
        get_node_definition,
        input_provider,
        node_definition_callback,
        signature,
        content,
        []() { return std::vector<std::string>(); }
    );
}

} // namespace Lab