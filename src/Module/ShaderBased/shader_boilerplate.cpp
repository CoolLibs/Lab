#include "shader_boilerplate.h"
#include "Cool/String/String.h"

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
        Cool::String::replace_all_beginnings_of_words_inplace(code, name + "(", name + "(coollab_context, ");
        Cool::String::replace_all_beginnings_of_words_inplace(code, name + "/*coollabdef*/(", name + "(CoollabContext coollab_context, ");
    }

    // Fixup the extra commas for functions that had no arguments initially
    Cool::String::replace_all_inplace(code, ", )", ")");
    Cool::String::replace_all_inplace(code, "(coollab_context, ()", "(coollab_context");

    return code;
}

auto generate_shader_code(
    Cool::NodesGraph const&                          graph,
    Cool::NodeId const&                              root_node_id,
    Cool::GetNodeDefinition_Ref<NodeDefinition>      get_node_definition,
    Cool::InputProvider_Ref                          input_provider,
    MaybeGenerateModule const&                       maybe_generate_module,
    FunctionSignature const&                         signature,
    ShaderContent const&                             content,
    std::function<std::vector<std::string>()> const& get_textures_names
)
    -> tl::expected<std::string, std::string>
{
    auto       context            = CodeGenContext{graph, get_node_definition, input_provider};
    auto const main_function_name = gen_desired_function(
        signature,
        root_node_id,
        context,
        maybe_generate_module
    );

    if (!main_function_name)
        return tl::make_unexpected(fmt::format("Failed to generate shader code:\n{}", main_function_name.error()));

    static std::string const global_uniforms =
        R"glsl(
uniform float _delta_time;
uniform float     _time;
uniform float     _height;
uniform float     _audio_volume;
uniform sampler1D _audio_spectrum;
uniform sampler1D _audio_waveform;
uniform mat3      _camera2D;
uniform mat3      _camera2D_inverse;
uniform sampler2D _previous_frame_texture;
uniform sampler2D mixbox_lut; // The uniform must have this exact name that mixbox.glsl expects.
)glsl";

    static std::string const global_includes =
        R"glsl(
#include "_ROOT_FOLDER_/res/shader-utils.glsl"
#include "_ROOT_FOLDER_/res/mixbox/mixbox.glsl"
#include "_COOL_RES_/shaders/math.glsl"
#include "_COOL_RES_/shaders/color_conversions.glsl"
#include "_COOL_RES_/shaders/Texture.glsl"
#include "_COOL_RES_/shaders/camera.glsl"
)glsl";

    static std::string const global_structurations =
        R"glsl(
vec2 to_view_space(vec2 uv)
{
    vec3 p = _camera2D_inverse * vec3(uv, 1.);
    return p.xy / p.z;
}
)glsl";

    std::string textures_uniforms;
    auto const  tex_names = get_textures_names();
    for (auto const& name : tex_names)
    {
        textures_uniforms += fmt::format("uniform sampler2D {};\n", name);
    }

    using fmt::literals::operator""_a;
    return fmt::format(
        FMT_COMPILE(R"glsl(
            {in_version}
            {global_uniforms}
            {in_uniforms}
            {textures_uniforms}
            {global_includes}
            {in_includes}
            {global_structurations}
            {in_structuration}
            {output_indices_declarations}
            {main_function_implementation}
            {in_main_content}
        )glsl"),
        "in_version"_a                   = content.version,
        "global_uniforms"_a              = global_uniforms,
        "in_uniforms"_a                  = content.uniforms,
        "textures_uniforms"_a            = textures_uniforms,
        "global_includes"_a              = global_includes,
        "in_includes"_a                  = content.includes,
        "global_structurations"_a        = global_structurations,
        "in_structuration"_a             = content.structuration,
        "output_indices_declarations"_a  = gen_all_output_indices_declarations(graph),
        "main_function_implementation"_a = inject_context_argument_in_all_functions(context.code(), context.function_names()),
        "in_main_content"_a              = content.main(*main_function_name)
    );
}

} // namespace Lab