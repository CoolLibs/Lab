#include "generate_simulation_shader_code.h"
#include <Nodes/PrimitiveType.h>
#include <optional>
#include "Nodes/shader_boilerplate.h"
#include "fmt/compile.h"
#include "microcode_generator.h"
#include "shader_code_constants.h"

namespace Lab {

auto constexpr structuration_template = FMT_COMPILE(R"glsl(
{buffers}
struct Particle{n}D
{{
    vec{n} position;
    vec{n} velocity;
    vec{n} acceleration;
    float size;
    float lifetime;
    uint id;
}};
)glsl");

auto constexpr main_template = FMT_COMPILE(R"glsl(
void cool_main()
{{
    uint     gid = gl_GlobalInvocationID.x;
    Particle{n}D particle;
    particle.position     = {position_b2v};
    particle.velocity     = {velocity_b2v};
    particle.acceleration = {acceler_zero};
    particle.size         = _sizes[gid];
    particle.lifetime     = _lifetimes[gid];
    particle.id           = gid;

    CoollabContext coollab_context;
    coollab_context.uv = particle.position;

    particle = {main_function_name}(coollab_context, particle);

    particle.velocity += particle.acceleration * _delta_time;
    particle.position += particle.velocity * _delta_time; // TODO(Particles) proper integration method

    {position_v2b}
    {velocity_v2b}
    _sizes[gid]              = particle.size;
    _lifetimes[gid]          = particle.lifetime;
}}
)glsl");

//

auto generate_simulation_shader_code(
    Cool::NodesGraph const&                     graph,
    Cool::NodeId const&                         root_node_id,
    Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition,
    Cool::InputProvider_Ref                     input_provider
)
    -> tl::expected<std::string, std::string>
{
    size_t const& n = 2;
    using fmt::literals::operator""_a;
    ShaderContent content{
        .version       = version,
        .uniforms      = uniforms,
        .includes      = includes,
        .structuration = fmt::format(
            structuration_template,
            "buffers"_a = buffers,
            "n"_a       = n
        ),
        .main = [&](
                    std::string const& main_function_name
                ) -> std::string {
            return fmt::format(
                main_template,
                "main_function_name"_a = main_function_name,
                "position_b2v"_a       = buffer_to_vec(n, "_positions", "gid"),
                "velocity_b2v"_a       = buffer_to_vec(n, "_velocities", "gid"),
                "position_v2b"_a       = vec_to_buffer(n, "_positions", "gid", "particle.position"),
                "velocity_v2b"_a       = vec_to_buffer(n, "_velocities", "gid", "particle.velocity"),
                "acceler_zero"_a       = vec_zero(n),
                "n"_a                  = n
            );
        },
    };

    FunctionSignature signature{
        .arity = 1,
    };

    assert(n == 2 || n == 3);
    switch (n)
    {
    case 2:
    {
        signature.from = PrimitiveType::Particle2D;
        signature.to   = PrimitiveType::Particle2D;
    }
    break;
    case 3:
    {
        signature.from = PrimitiveType::Particle3D;
        signature.to   = PrimitiveType::Particle3D;
    }
    break;
    default: break; // that was asserted out
    }

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
