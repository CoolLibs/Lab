#include "generate_compositing_shader_code.h"
#include "Nodes/shader_boilerplate.h"

namespace Lab {

auto generate_compositing_shader_code(
    Cool::NodesGraph const&                          graph,
    Cool::NodeId const&                              root_node_id,
    Cool::GetNodeDefinition_Ref<NodeDefinition>      get_node_definition,
    Cool::InputProvider_Ref                          input_provider,
    NodeDefinitionCallback const&                    node_definition_callback,
    std::function<std::vector<std::string>()> const& get_textures_names
)
    -> tl::expected<std::string, std::string>
{
    using fmt::literals::operator""_a;
    ShaderContent content{
        .version       = "#version 410",
        .uniforms      = R"glsl(
uniform sampler2D mixbox_lut; // The uniform must have this exact name that mixbox.glsl expects.

out vec4 out_Color;
)glsl",
        .includes      = R"glsl(
#include "_ROOT_FOLDER_/res/mixbox/mixbox.glsl"
)glsl",
        .structuration = R"glsl(
vec2 to_view_space(vec2 uv)
{
    vec3 p = _camera2D_inverse * vec3(uv, 1.);
    return p.xy / p.z;
}
)glsl",
        .main          = [](
                    std::string const& main_function_name
                ) -> std::string {
            return fmt::format(
                FMT_COMPILE(R"glsl(
void main()
{{
    vec2 uv = normalized_uv();
    vec3 tmp = _camera2D * vec3(uv, 1.);
    uv = tmp.xy / tmp.z;
    CoollabContext coollab_context;
    coollab_context.uv = uv;
    out_Color = {main_function_name}(coollab_context, uv);
}}
)glsl"),
                "main_function_name"_a = main_function_name
            );
        },
    };

    FunctionSignature signature{
        .from  = PrimitiveType::UV,
        .to    = PrimitiveType::sRGB_StraightA, // We output sRGB and straight alpha because this is what the rest of the world expects most of the time.
        .arity = 1,
    };

    return generate_shader_code(
        graph,
        root_node_id,
        get_node_definition,
        input_provider,
        node_definition_callback,
        signature,
        content,
        get_textures_names
    );
}

} // namespace Lab