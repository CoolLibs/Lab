#include "CodeGen.h"
#include <Cool/String/String.h>
#include <numeric>
#include <sstream>

namespace CodeGen {

static constexpr const char* default_sdf = R"(float is0_default_sdf(vec3 pos) {
    return MAX_DIST;
}

)";

static constexpr const char* ray_marcher_begin = R"(#version 430
layout(location = 0) in vec2 _uv;
uniform float _time;
out vec4 out_Color;
// clang-format off
#include "_COOL_RES_/shaders/camera.glsl"
#include "_COOL_RES_/shaders/pbr_calc.glsl"
#include "_COOL_RES_/shaders/math.glsl"
#include "_COOL_RES_/shaders/iqnoise_3D.glsl" 
#include "is0 shaders/light.glsl"
#include "is0 shaders/hg_sdf.glsl" 
// clang-format on
// ----- Ray marching options ----- //
#define MAX_STEPS 1500
#define MAX_DIST 200.
#define SURF_DIST 0.0001
#define NORMAL_DELTA 0.0001
#define DONT_INVERT_SDF 1.
#define INVERT_SDF -1.

struct RayMarchRes {
    float distance;
    int iterations_count;
    vec3 ray_direction;
    vec3 hit_position;
    vec3 normal;
};

struct MaterialData {
    //Material
    vec3  material_color_base;
    vec3  material_color_diffuse;
    vec3  material_color_specular;
    float material_roughtnes;

    //Fresnel
    float fresnel_strength;
    vec3  fresnel_color;

    //Refraction
    float refraction_Density;
    float refraction_abberation;
    float refraction_density;

    //Glow
    float glow_strength;
    vec3  glow_color;
};
)";

static constexpr const char* ray_marcher = R"(
vec3 get_normal(vec3 p) {
    const float h = NORMAL_DELTA;
	const vec2 k = vec2(1., -1.);
    return normalize( k.xyy * is0_main_sdf( p + k.xyy*h ) + 
                      k.yyx * is0_main_sdf( p + k.yyx*h ) + 
                      k.yxy * is0_main_sdf( p + k.yxy*h ) + 
                      k.xxx * is0_main_sdf( p + k.xxx*h ) );
}

RayMarchRes rayMarching(vec3 ro, vec3 rd, float in_or_out) {
    float t = 0.;
 	int i = 0;
    for (i; i < MAX_STEPS; i++) {
    	vec3 pos = ro + rd * t;
        float d = is0_main_sdf(pos) * in_or_out;
        t += d;
        // If we are very close to the object, consider it as a hit and exit this loop
        if( t > MAX_DIST || abs(d) < SURF_DIST*0.99) break;
    }
    vec3 final_pos = ro + rd * t;
    return RayMarchRes(t, i, rd, final_pos, get_normal(final_pos));
}
vec3 render(vec3 ro, vec3 rd) {
    vec3 finalCol = vec3(0.3, 0.7, 0.98);
    
    RayMarchRes res = rayMarching(ro, rd, DONT_INVERT_SDF);
    float d = res.distance;
    float iterations_count = res.iterations_count;
    vec3 p = res.hit_position;
    vec3 normal = res.normal;
    if (d < MAX_DIST) {
      finalCol = normal * 0.5 + 0.5;

)";

static constexpr const char* ray_marcher_end = R"(

finalCol = saturate(finalCol);
    finalCol = pow(finalCol, vec3(0.4545)); // Gamma correction
    return finalCol;
}
void main() {
    vec3 ro = cool_ray_origin();
    vec3 rd = cool_ray_direction();
    out_Color = vec4(render(ro, rd), 1.);
}
)";

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
    return ray_marcher_begin +
           nodes_extra_code(node_templates) +
           code_gen_render_effects_extra_code(effects) +
           std::string{default_sdf} +
           main_sdf(node_tree, node_templates) +
           ray_marcher +
           code_gen_render_effects(effects.for_objects) +
           "}" +
           code_gen_render_effects(effects.always_applied) +
           ray_marcher_end;
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