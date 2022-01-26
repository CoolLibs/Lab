#include "CodeGen.h"
#include <Cool/String/String.h>
#include <numeric>
#include <sstream>

namespace CodeGen {

static constexpr const char* default_sdf = R"(float is0_default_sdf(vec3 pos) {
    return MAX_DIST;
}

)";

static std::string glsl_version()
{
    return "#version " + std::to_string(COOL_OPENGL_VERSION) + '\n';
}

static constexpr const char* input_and_output_parameters = R"(
layout(location = 0) in vec2 _uv;
uniform float _time;
out vec4 out_Color;
)";

static std::string includes()
{
    return R"(
        #include "_COOL_RES_/shaders/camera.glsl"
        #include "_COOL_RES_/shaders/iqnoise_3D.glsl" 
        #include "_COOL_RES_/shaders/math.glsl"
        #include "_COOL_RES_/shaders/pbr_calc.glsl"
        #include "is0 shaders/hg_sdf.glsl" 
        #include "is0 shaders/light.glsl"
    )";
}

static std::string ray_marcher_parameters()
{
    return "float MAX_DIST = cool_camera_far_plane; \n";
}

static std::string apply_function(const std::string& type_and_function_names, BaseCode base_code)
{
    return "\n" + base_code.extra_code +
           type_and_function_names +
           base_code.parameters_declaration +
           "{" + code_gen_base_code(base_code) + "}\n";
}

static std::string apply_material(const std::vector<RenderEffect>& render_effects)
{
    return R"(
        vec3 apply_material(RayMarchRes res){
            vec3 material_color = vec3(0., 0., 0.);)" +
           code_gen_render_effects(render_effects) +
           R"(return material_color;
        }
    )";
}

static std::string post_process(const std::vector<RenderEffect>& render_effects)
{
    return R"(
        vec3 post_process(RayMarchRes res, vec3 color){)" +
           code_gen_render_effects(render_effects) +
           R"(color = saturate(color);
            color = pow(color, vec3(0.4545)); // Gamma correction
            return color;
        }
    )";
}

static std::string main()
{
    return R"(
        void main() {
            vec3 ro = cool_ray_origin();
            vec3 rd = cool_ray_direction();
            RayMarchRes res = rayMarching(ro, rd, DONT_INVERT_SDF);
            vec3 color = vec3(0., 0., 0.);
            if (res.distance < MAX_DIST){
                color += apply_material(res);
            }
            else {
                color += apply_background(res);
            }
            color = post_process(res,color);
            out_Color = vec4(color, 1.);
        }
    )";
}

static auto compute_sdf_identifiers(const Node& node, const NodeTemplate& node_template, const NodeTree& node_tree) -> std::vector<std::pair<std::string, std::string>>
{
    using namespace std::string_literals;
    std::vector<std::pair<std::string, std::string>> sdf_identifiers;
    sdf_identifiers.reserve(node.input_pins.size());
    for (size_t i = 0; i < node.input_pins.size(); ++i) {
        const Node* input_node = node_tree.find_input_node(node.input_pins[i]);
        sdf_identifiers.push_back(std::make_pair(
            node_template.sdf_identifiers[i],
            input_node ? function_name({input_node->node_template_name, input_node->id}) : "is0_default_sdf"s));
    }
    return sdf_identifiers;
}

static const NodeTemplate& find_node_template(const Node& node, const std::vector<NodeTemplate>& node_templates)
{
    return *std::ranges::find_if(node_templates, [&](const NodeTemplate& node_template) {
        return node_template.name == node.node_template_name;
    });
}

static auto nodes_extra_code(const std::vector<NodeTemplate>& node_templates) -> std::string
{
    return std::accumulate(
        node_templates.begin(), node_templates.end(), std::string{}, [](const std::string& acc, const NodeTemplate& node_template) {
            return acc + node_template.extra_code;
        });
}

std::string full_shader_code(const NodeTree& node_tree, const std::vector<NodeTemplate>& node_templates, const RenderEffects& effects)
{
    return glsl_version() +
           input_and_output_parameters +
           includes() +
           ray_marcher_parameters() +
           std::string{default_sdf} +
           main_sdf(node_tree, node_templates) +
           apply_function("vec3 get_normal", effects.normal[effects.normal_index]) +
           nodes_extra_code(node_templates) +
           code_gen_render_effects_extra_code(effects) +
           apply_function("RayMarchRes rayMarching", effects.ray_marching[effects.ray_index]) +
           apply_material(effects.for_objects) +
           apply_function("vec3 apply_background", effects.background[effects.background_index]) +
           post_process(effects.post_processing) +
           main();
}

std::string main_sdf(const NodeTree& node_tree, const std::vector<NodeTemplate>& node_templates)
{
    std::stringstream declarations;
    std::stringstream definitions;
    std::stringstream main_sdf_definition;
    main_sdf_definition << R"(float is0_main_sdf(vec3 pos) {
    float d = MAX_DIST;)";

    for (const auto& node : node_tree.nodes) {
        const auto& node_template       = find_node_template(node, node_templates);
        const auto  fn_signature_params = FnSignatureParams{.fn_name_params = FnNameParams{
                                                                .node_template_name = node.node_template_name,
                                                                .node_id            = node.id},
                                                            .sdf_param_declaration = node_template.vec3_input_declaration};
        declarations << function_declaration(fn_signature_params) << '\n';
        definitions << function_definition(FnDefinitionParams{
            .fn_signature_params = fn_signature_params,
            .body                = function_body(node.parameter_list,
                                                 node_template.code_template,
                                                 compute_sdf_identifiers(node, node_template, node_tree))});
        definitions << "\n\n";
        if (node_tree.has_no_successor(node)) {
            main_sdf_definition << "\n    d = min(d, " << function_name({node.node_template_name, node.id}) << "(pos));";
        }
    }
    main_sdf_definition << R"(
    return d;
})";
    return declarations.str() + '\n' + definitions.str() + main_sdf_definition.str();
}

std::string convert_to_valid_glsl_name(std::string name)
{
    Cool::String::replace_all(name, " ", "_");
    Cool::String::replace_all(name, "-", "_");
    return name;
}

std::string function_name(const FnNameParams& p)
{
    return convert_to_valid_glsl_name(std::string{p.node_template_name} + std::to_string(*p.node_id));
}

std::string function_signature(const FnSignatureParams& p)
{
    return "float " + function_name(p.fn_name_params) + std::string{p.sdf_param_declaration};
}

std::string function_declaration(const FnSignatureParams& p)
{
    return function_signature(p) + ";";
}

std::string function_definition(const FnDefinitionParams& p)
{
    return function_signature(p.fn_signature_params) + std::string{p.body};
}

std::string function_body(const Cool::ParameterList& list, const std::string& code_template, const std::vector<std::pair<std::string, std::string>>& sdf_identifiers)
{
    return "{\n" +
           parameters_definitions(list) +
           Cool::String::replace({code_template, sdf_identifiers, "${", "}"}) +
           "\n}";
}

std::string parameters_definitions(const Cool::ParameterList& list)
{
    return std::accumulate(list->begin(), list->end(), std::string{""}, [](const std::string& acc, const auto& param) {
        return acc + "    " + parameter_definition_any(param) + '\n';
    });
}

std::string parameter_definition_any(const Cool::Parameter::Any& param)
{
    return std::visit([](auto&& param) { return parameter_definition(param); }, param);
}
} // namespace CodeGen