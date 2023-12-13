#include "generate_simulation_shader_code.h"
#include <optional>
#include "Nodes/shader_boilerplate.h"
#include "microcode_generator.h"

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
    ShaderContent        content{
               .version       = "",
               .uniforms      = R"glsl(
            uniform float _particle_size;
        )glsl",
               .includes      = R"glsl(
        )glsl",
               .structuration = R"glsl(
            layout(std430, binding = 0) buffer _positions_buffer
            {
                float _positions[];
            };

            layout(std430, binding = 1) buffer _velocities_buffer
            {
                float _velocities[];
            };

            layout(std430, binding = 2) buffer _sizes_buffer
            {
                float _sizes[];
            };

            layout(std430, binding = 3) buffer _lifetimes_buffer
            {
                float _lifetimes[];
            };

            struct Particle
            {
                vec2 position;
                vec2 velocity;
                vec2 acceleration;
                float size;
                float lifetime;
                uint id;
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
                )glsl"),
                        "main_function_name"_a = main_function_name,
                        "position_b2v"_a = buffer_to_vec(2, "_positions", "gid"),
                        "velocity_b2v"_a = buffer_to_vec(2, "_velocities", "gid"),
                        "acceler_zero"_a = vec_zero(2),
                        "position_v2b"_a = vec_to_buffer(2, "_positions", "gid", "particle.position"),
                        "velocity_v2b"_a = vec_to_buffer(2, "_velocities", "gid", "particle.velocity")
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

    //     using fmt::literals::operator""_a;
    //     return fmt::format(
    //         FMT_COMPILE(R"glsl(
    // uniform float _delta_time;
    // uniform float     _time;
    // uniform float     _height;
    // uniform float     _audio_volume;
    // uniform sampler1D _audio_spectrum;
    // uniform sampler1D _audio_waveform;
    // uniform mat3      _camera2D;
    // uniform mat3      _camera2D_inverse;
    // uniform sampler2D _previous_frame_texture;
    // uniform sampler2D _particles_texture;

    // #include "_ROOT_FOLDER_/res/shader-utils.glsl"
    // #include "_COOL_RES_/shaders/math.glsl"
    // #include "_COOL_RES_/shaders/color_conversions.glsl"
    // #include "_COOL_RES_/shaders/Texture.glsl"
    // #include "_COOL_RES_/shaders/camera.glsl"

    // layout(std430, binding = 0) buffer _positions_buffer
    // {{
    //     float _positions[];
    // }};

    // layout(std430, binding = 1) buffer _velocities_buffer
    // {{
    //     float _velocities[];
    // }};

    // struct Particle
    // {{
    //     vec2 position;
    //     vec2 velocity;
    //     vec2 acceleration;
    // }};

    // struct CoollabContext
    // {{
    //     vec2 uv;
    // }};

    // {output_indices_declarations}

    // {main_function_implementation}

    // void cool_main()
    // {{
    //     uint     gid = gl_GlobalInvocationID.x;
    //     Particle particle;
    //     particle.position     = vec2(_positions[gid * 2], _positions[gid * 2 + 1]);
    //     particle.velocity     = vec2(_velocities[gid * 2], _velocities[gid * 2 + 1]);
    //     particle.acceleration = vec2(0.);

    //     CoollabContext coollab_context;
    //     coollab_context.uv = particle.position;

    //     particle = {main_function_name}(coollab_context, particle);

    //     particle.velocity += particle.acceleration * _delta_time;
    //     particle.position += particle.velocity * _delta_time; // TODO(Particles) proper integration method

    //     _positions[gid * 2]      = particle.position.x;
    //     _positions[gid * 2 + 1]  = particle.position.y;
    //     _velocities[gid * 2]     = particle.velocity.x;
    //     _velocities[gid * 2 + 1] = particle.velocity.y;
    // }}
    // )glsl"
    //         ),
    //         "output_indices_declarations"_a  = gen_all_output_indices_declarations(graph),
    //         "main_function_implementation"_a = inject_context_argument_in_all_functions(context.code(), context.function_names()),
    //         "main_function_name"_a           = *main_function_name
    //     );
}

} // namespace Lab
