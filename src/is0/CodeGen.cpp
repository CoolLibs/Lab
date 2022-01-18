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
#include "is0 shaders/light.glsl"
// clang-format on

// ----- Ray marching options ----- //
#define MAX_STEPS 1500
#define MAX_DIST 200.
#define SURF_DIST 0.0001
#define NORMAL_DELTA 0.0001
#define DONT_INVERT_SDF 1.
#define INVERT_SDF -1.

#define saturate(v) clamp(v, 0., 1.)

float smooth_min(float f1, float f2, float strength) {
    float h = clamp(0.5 +0.5*(f2-f1)/strength, 0.0, 1.0);
    return mix(f2, f1, h) - strength*h*(1.0-h);
}

float smooth_max(float f1, float f2, float strength) {
    float h = clamp(0.5 - 0.5*(f2-f1)/strength, 0.0, 1.0);
    return mix(f2, f1, h) + strength*h*(1.0-h);
}


float hash(vec3 x)
{
    // based on: pcg3 by Mark Jarzynski: http://www.jcgt.org/published/0009/03/02/
    uvec3 v = uvec3(x * 8192.0) * 1664525u + 1013904223u;
    v += v.yzx * v.zxy;
    v ^= v >> 16u;
    return float(v.x + v.y * v.z) * (1.0 / float(0xffffffffu));
}

float sph(vec3 i, vec3 f, vec3 c){
    float rad = 0.5*hash_0_to_1(i+c);
    return length(f-vec3(c)) - rad;
}
struct RayMarchRes {
    float dist;
    int iterations_count;
    int render;
};

)";

static std::string render(const RenderEffects& effects)
{
    return R"(
vec3 render_effects(vec3 ro, vec3 rd) {
    vec3 finalCol = vec3(0.3, 0.7, 0.98);
    RayMarchRes res              = rayMarching(ro, rd, DONT_INVERT_SDF);
    float       d                = res.dist;
    float       iterations_count = res.iterations_count;
    if (d < MAX_DIST) {
        vec3 p      = ro + rd * d;
        vec3 normal = getNormal(p);
        finalCol = normal * 0.5 + 0.5;
        )" +
           code_gen_effects_object(effects) +
           "}" +
           code_gen_effects_world(effects) + R"(
    finalCol = saturate(finalCol);
    finalCol = pow(finalCol, vec3(0.4545)); // Gamma correction
    return finalCol;
}

)";
}

static constexpr const char* ray_marcher_impl = R"(
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
    return RayMarchRes(t,i,0);
}
vec3 getNormal(vec3 p) {
    const float h = NORMAL_DELTA;
	const vec2 k = vec2(1., -1.);
    return normalize( k.xyy * is0_main_sdf( p + k.xyy*h ) +
                      k.yyx * is0_main_sdf( p + k.yyx*h ) +
                      k.yxy * is0_main_sdf( p + k.yxy*h ) +
                      k.xxx * is0_main_sdf( p + k.xxx*h ) );
}
)";

static constexpr const char* ray_marcher_end = R"(

vec3 render(vec3 ro, vec3 rd) {
    vec3 finalCol = vec3(0.3, 0.7, 0.98);
    
    float d = rayMarching(ro, rd);
    
    if (d < MAX_DIST) {
      vec3 p = ro + rd * d;
      vec3 normal = getNormal(p); 
      //vec3 ref = reflect(rd, normal);
      
      //float sunFactor = saturate(dot(normal, nSunDir));
      //float sunSpecular = pow(saturate(dot(nSunDir, ref)), specularStrength); // Phong
    
      finalCol = normal * 0.5 + 0.5;
    }
    
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

std::string full_shader_code(const NodeTree& node_tree, const std::vector<NodeTemplate>& node_templates, const RenderEffects& effects, const LightProperties& light, const MaterialProperties& material)
{
    return ray_marcher_begin +
           code_gen_effects_parameters(effects) +
           std::string{default_sdf} +
           main_sdf(node_tree, node_templates) +
           ray_marcher_impl +
           CodeGen::addSmoke(effects.smoke) +
           render(effects) +
           pbr_renderer_codegen(light, material) +
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
    return convert_to_valid_glsl_name(std::string{p.node_template_name} +
                                      "_" + std::to_string(*p.node_id));
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