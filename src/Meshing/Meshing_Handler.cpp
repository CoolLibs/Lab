#include "Meshing_Handler.h"
#include <Cool/Nodes/NodesGraph.h>
#include <fmt/format.h>
#include "Cool/Gpu/OpenGL/ComputeShader.h"
#include "Cool/Log/Debug.h"
#include "Cool/Log/OptionalErrorMessage.h"
#include "Cool/Log/ToUser.h"
#include "Cool/Nodes/GetNodeDefinition_Ref.h"
#include "Module/ShaderBased/generate_shader_code.h"
#include "Module/ShaderBased/set_uniforms_for_shader_based_module.h"
#include "marching_cube.hpp"
#include "meshing_io.hpp"

namespace Lab {

static auto generate_compute_shader(std::string const& shader_code) -> tl::expected<Cool::OpenGL::ComputeShader, Cool::OptionalErrorMessage>
{
    try
    {
        return Cool::OpenGL::ComputeShader{64, shader_code};
    }
    catch (Cool::Exception const& e)
    {
        return tl::make_unexpected(e.error_message());
    }
}

static auto generate_meshing_shader_code(
    Cool::NodesGraph const&                     graph,
    Cool::NodeId const&                         root_node_id,
    Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition
) -> tl::expected<std::string, std::string>
{
    using fmt::literals::operator""_a;
    auto const content = ShaderCodeBits{
        .version     = "",
        .before_main = R"glsl(
uniform float _step_size;
uniform float _box_size;

layout(std430, binding = 0) buffer _signed_distance_field_buffer
{
    float _signed_distance_field[];
};

struct CoollabContext
{
    vec2 uv;
};
        )glsl",
        .make_main   = [](std::string const& main_function_name) {
            return fmt::format(FMT_COMPILE(R"glsl(
void cool_main()
{{
    uint gid = gl_GlobalInvocationID.x +
        gl_GlobalInvocationID.y * DispatchSize.x + 
        gl_GlobalInvocationID.z * DispatchSize.y * DispatchSize.y;

    vec3 pos = vec3(-_box_size/2. + gl_GlobalInvocationID*_step_size);

    CoollabContext dummy_coollab_context;
    dummy_coollab_context.uv = vec2(0);
    _signed_distance_field[gid] = {main_function_name}(dummy_coollab_context, pos);
    // _signed_distance_field[gid] = pos.x;
}}
                )glsl"),
                                 "main_function_name"_a = main_function_name);
        },
    };

    auto const main_function_signature = FunctionSignature{
        .from  = PrimitiveType::Vec3,
        .to    = PrimitiveType::SignedDistance,
        .arity = 1,
    };

    return generate_shader_code(
        graph,
        root_node_id,
        get_node_definition,
        [](auto const&, auto const&) -> std::optional<std::string> { return std::nullopt; },
        main_function_signature,
        content,
        []() { return std::vector<std::string>{}; }
    );
}

Meshing_Handler::Meshing_Handler()
{
    bind_SSBO();
    _signed_distance_field.upload_data(get_ssbo_size(), nullptr);
}

void Meshing_Handler::imgui_window(meshing_imgui_window_Params const& meshing_imgui_params)
{
    _gui->imgui_window(_meshing_params, [&](std::filesystem::path const& path) {
        compute_mesh(
            meshing_imgui_params.feedback_double_buffer,
            meshing_imgui_params.nodes_graph,
            meshing_imgui_params.get_node_definition,
            meshing_imgui_params.project_system_values,
            _target_node_id,
            path
        );
    });
}

void Meshing_Handler::open_meshing_window(Cool::NodeId const& node_id)
{
    _target_node_id = node_id;
    _gui->open_window();
}

void Meshing_Handler::update_buffer_size()
{
    const size_t& ssbo_size{get_ssbo_size()};
    if (_ssbo_size != ssbo_size)
    {
        _ssbo_size = ssbo_size;
        bind_SSBO();
        _signed_distance_field.upload_data(_ssbo_size, nullptr);
    }
}

void Meshing_Handler::compute_mesh(
    Cool::DoubleBufferedRenderTarget const&     feedback_double_buffer,
    Cool::NodesGraph const&                     nodes_graph,
    Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition,
    SystemValues const&                         system_values,
    Cool::NodeId const&                         node_id,
    std::filesystem::path const&                path
)
{
    if constexpr (COOL_OPENGL_VERSION < 430)
    {
        Cool::Log::ToUser::warning("Meshing", "OpenGL version is lower than 430. Meshing need compute shaders, which are not supported by this version.");
        return;
    }

    update_buffer_size();

    auto shader_code{generate_meshing_shader_code(nodes_graph, node_id, get_node_definition)};

    if (!shader_code)
    {
        Cool::Log::ToUser::error("Meshing", fmt::format("Unable to generate the shader code: {}", shader_code.error()));
        return;
    }

    auto meshing_compute_shader{generate_compute_shader(*shader_code)};

    if (!meshing_compute_shader)
    {
        Cool::Log::ToUser::error("Meshing", fmt::format("Unable to compile the compute shader: {}", meshing_compute_shader.error().error_message()));
        // ImGui::SetClipboardText(shader_code->c_str());
        return;
    }

    // TODO(Meshing) expose those parameters
    const float boxSize           = 2.f;
    const float meshing_step_size = boxSize / static_cast<float>(_meshing_params.sampling_count.x - 1);

    meshing_compute_shader->bind();
    bind_SSBO();
    meshing_compute_shader->set_uniform("_box_size", boxSize);
    meshing_compute_shader->set_uniform("_step_size", meshing_step_size);

    set_uniforms_for_shader_based_module(meshing_compute_shader.value(), system_values, {}, feedback_double_buffer, nodes_graph);

    meshing_compute_shader->compute(_meshing_params.sampling_count);

    // CPU get data back
    std::vector<float> sdf_sampling{};
    sdf_sampling.resize(_ssbo_size);

    // need mutable keyword to call download_data in const function
    _signed_distance_field.download_data(sdf_sampling);

    // test print data
    // for (size_t i = 0; i < sdf_sampling.size(); i++)
    // {
    //     std::string s = "v[" + std::to_string(i) + "] = " + std::to_string(sdf_sampling[i]);
    //     Cool::Log::Debug::info("Meshing", s);
    // }

    auto mesh{Meshing::mesh_from_sdf_sampling(sdf_sampling, boxSize, meshing_step_size, _meshing_params.sampling_count.x)};

    // TODO [Meshing] : dispatch depending on the export format in path extension
    Meshing::write_to_ply(mesh, path);

    Cool::Log::Debug::info("Meshing", fmt::format("Mesh exported to  {}", path.string()));
}

} // namespace Lab
