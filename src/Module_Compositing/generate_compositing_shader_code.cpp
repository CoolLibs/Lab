#include "generate_compositing_shader_code.h"
#include "Module/ShaderBased/shader_boilerplate.h"

namespace Lab {

auto generate_compositing_shader_code(
    Cool::NodesGraph const&                          graph,
    Cool::NodeId const&                              root_node_id,
    Cool::GetNodeDefinition_Ref<NodeDefinition>      get_node_definition,
    Cool::InputProvider_Ref                          input_provider,
    MaybeGenerateModule const&                       maybe_generate_module,
    std::function<std::vector<std::string>()> const& get_textures_names
) -> tl::expected<std::string, std::string>
{
    using fmt::literals::operator""_a;
    auto const content = ShaderContent{
        .version     = "#version 410",
        .before_main = R"glsl(
out vec4 out_Color;
struct CoollabContext
{
    vec2 uv;
};
        )glsl",
        .make_main   = [](std::string const& main_function_name) {
            return fmt::format(FMT_COMPILE(R"glsl(
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
                                 "main_function_name"_a = main_function_name);
        },
    };

    auto const signature = FunctionSignature{
        .from  = PrimitiveType::UV,
        .to    = PrimitiveType::sRGB_StraightA, // We output sRGB and straight alpha because this is what the rest of the world expects most of the time.
        .arity = 1,
    };

    return generate_shader_code(
        graph,
        root_node_id,
        get_node_definition,
        input_provider,
        maybe_generate_module,
        signature,
        content,
        get_textures_names
    );
}

} // namespace Lab