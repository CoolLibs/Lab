#include "Meshing_Handler.h"
#include <Cool/Nodes/NodesGraph.h>
#include <fmt/format.h>
#include <Meshing/meshing_types.hpp>
#include <optional>
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

static auto ssbo_size_from_sampling_count(Meshing::MeshingParams const& meshing_params) -> unsigned int
{
    return glm::compMul(meshing_params.sampling_count);
}

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

void Meshing_Handler::imgui_window(meshing_imgui_window_Params const& meshing_imgui_params)
{
    _gui->imgui_window(_meshing_params, [&](meshing_export_mesh_Params const& meshing_export_params) {
        const std::optional<Meshing::Mesh> optional_mesh{compute_mesh(
            meshing_imgui_params.feedback_double_buffer,
            meshing_imgui_params.nodes_graph,
            meshing_imgui_params.get_node_definition,
            meshing_imgui_params.project_system_values,
            _target_node_id
        )};

        if (!optional_mesh)
        {
            // Warning already logged in compute_mesh
            return;
        }

        const Meshing::Mesh& mesh{optional_mesh.value()};

        const std::filesystem::path path{meshing_export_params.file_path()};
        if (!std::filesystem::exists(path.parent_path()))
        {
            std::filesystem::create_directories(path.parent_path());
        }

        Meshing::write_to_file(mesh, path, Meshing::MeshExportFormat::PLY);
        Cool::Log::Debug::info("Meshing", fmt::format("Mesh exported to  {}", path.string()));
    });
}

void Meshing_Handler::open_meshing_window(Cool::NodeId const& node_id)
{
    _target_node_id = node_id;
    _gui->open_window();
}

std::optional<Meshing::Mesh> Meshing_Handler::compute_mesh(
    Cool::DoubleBufferedRenderTarget const&     feedback_double_buffer,
    Cool::NodesGraph const&                     nodes_graph,
    Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition,
    SystemValues const&                         system_values,
    Cool::NodeId const&                         node_id
) const
{
    if constexpr (COOL_OPENGL_VERSION < 430)
    {
        Cool::Log::ToUser::warning("Meshing", "OpenGL version is lower than 430. Meshing need compute shaders, which are not supported by this version.");
        return std::nullopt;
    }

    Cool::SSBO<float> signed_distance_field_ssbo{_ssbo_binding};
    signed_distance_field_ssbo.bind();
    const size_t ssbo_size{ssbo_size_from_sampling_count(_meshing_params)};
    signed_distance_field_ssbo.upload_data(ssbo_size, nullptr);

    auto shader_code{generate_meshing_shader_code(nodes_graph, node_id, get_node_definition)};

    if (!shader_code)
    {
        Cool::Log::ToUser::error("Meshing", fmt::format("Unable to generate the shader code: {}", shader_code.error()));
        return std::nullopt;
    }

    auto meshing_compute_shader{generate_compute_shader(*shader_code)};

    if (!meshing_compute_shader)
    {
        Cool::Log::ToUser::error("Meshing", fmt::format("Unable to compile the compute shader: {}", meshing_compute_shader.error().error_message()));
        // ImGui::SetClipboardText(shader_code->c_str());
        return std::nullopt;
    }

    // TODO(Meshing) expose those parameters
    const float boxSize           = 2.f;
    const float meshing_step_size = boxSize / static_cast<float>(_meshing_params.sampling_count.x - 1);

    meshing_compute_shader->bind();
    meshing_compute_shader->set_uniform("_box_size", boxSize);
    meshing_compute_shader->set_uniform("_step_size", meshing_step_size);

    set_uniforms_for_shader_based_module(meshing_compute_shader.value(), system_values, {}, feedback_double_buffer, nodes_graph);

    meshing_compute_shader->compute(_meshing_params.sampling_count);

    // CPU get data back
    std::vector<float> sdf_sampling{};
    sdf_sampling.resize(ssbo_size);

    signed_distance_field_ssbo.download_data(sdf_sampling);
    return Meshing::mesh_from_sdf_sampling(sdf_sampling, boxSize, meshing_step_size, _meshing_params.sampling_count.x);
}

} // namespace Lab
