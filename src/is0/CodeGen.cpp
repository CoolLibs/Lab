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
#include "_COOL_RES_/shaders/camera.glsl"

// ----- Ray marching options ----- //
#define MAX_STEPS 1500
#define MAX_DIST 200.
#define SURF_DIST 0.0001
#define NORMAL_DELTA 0.0001

#define saturate(v) clamp(v, 0., 1.)


)";

static constexpr const char* ray_marcher_def   = R"(

float softshadow2(in vec3 ro, in vec3 rd, float mint, float maxt, float k)
{   
    float res = 1.0;
    float ph  = 1e20;
    for (float t = mint; t < maxt;) {
        float h = is0_main_sdf(ro + rd * t);
        if (h < 0.001)
            return 0.0;
        float y = h * h / (2.0 * ph);
        float d = sqrt(h * h - y * y);
        res     = min(res, k * d / max(0.0, t - y));
        ph      = h;
        t += h;
    }
    return res;
}

float rayMarching(vec3 ro, vec3 rd) {
    float t = 0.;
    for (int i = 0; i < MAX_STEPS; i++) {
    	vec3 pos = ro + rd * t;
        float d = is0_main_sdf(pos);
        t += d;
        // If we are very close to the object, consider it as a hit and exit this loop
        if( t > MAX_DIST || abs(d) < SURF_DIST*0.99) break;
    }
    return t;
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
static constexpr const char* ray_marcher_end_1 = R"(
vec3 render(vec3 ro, vec3 rd) {
    vec3 finalCol = vec3(0.3, 0.7, 0.98);
    
    float d = rayMarching(ro, rd);
    vec3 p= ro + rd * d;
    if (d < MAX_DIST) {
      vec3 normal = getNormal(p); 
      //vec3 ref = reflect(rd, normal);
      
      //float sunFactor = saturate(dot(normal, nSunDir));
      //float sunSpecular = pow(saturate(dot(nSunDir, ref)), specularStrength); // Phong
      finalCol = normal * 0.5 + 0.5;
      }
    
)";
static constexpr const char* ray_marcher_end_2 = R"(
      
    
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

static constexpr const char* soft_shadows = R"(

vec3  sun  = vec3(100.0, 100.0, 100.0);
vec3 lights[] = {sun};
float shad = 1;
for(int i = 0; i < lights.length(); i++ ) {
    shad = shad * softshadow2(p, normalize(lights[i] - p), 0.01, MAX_DIST, 16);
    }
//finalCol = finalCol * shad;
)";

static constexpr const char* inigo_ao = R"(

//finalCol = finalCol * calcOcclusion(p, getNormal(p), 0.1);
)";

static constexpr const char* poisson_ao  = R"(

//finalCol = finalCol * ssao(p);
)";
static constexpr const char* poisson_pts = R"(

vec3 BigRand[32] = {
    vec3(0.656178f, 0.785943f, 0.0918607f),
    vec3(0.0980255f, 0.770562f, 0.888882f),
    vec3(0.353252f, 0.255623f, 0.0786767f),
    vec3(0.618091f, 0.510575f, 0.594409f),
    vec3(0.0362255f, 0.71392f, 0.369793f),
    vec3(0.948057f, 0.122684f, 0.21366f),
    vec3(0.986175f, 0.0407117f, 0.76928f),
    vec3(0.248604f, 0.0666524f, 0.984375f),
    vec3(0.972198f, 0.96704f, 0.662496f),
    vec3(0.635456f, 0.410657f, 0.909421f),
    vec3(0.894436f, 0.695456f, 0.351756f),
    vec3(0.990631f, 0.0898465f, 0.475967f),
    vec3(0.0231635f, 0.0941496f, 0.698111f),
    vec3(0.812677f, 0.327525f, 0.700522f),
    vec3(0.582232f, 0.955535f, 0.728751f),
    vec3(0.650258f, 0.0143742f, 0.585223f),
    vec3(0.0383618f, 0.523209f, 0.759117f),
    vec3(0.446425f, 0.650929f, 0.331828f),
    vec3(0.106143f, 0.437605f, 0.248207f),
    vec3(0.710746f, 0.0986663f, 0.133061f),
    vec3(0.974395f, 0.309458f, 0.915311f),
    vec3(0.729423f, 0.975402f, 0.962401f),
    vec3(0.126102f, 0.0279855f, 0.415845f),
    vec3(0.291726f, 0.862941f, 0.545305f),
    vec3(0.909604f, 0.409406f, 0.475478f),
    vec3(0.0169683f, 0.193884f, 0.221442f),
    vec3(0.388836f, 0.962859f, 0.175787f),
    vec3(0.160375f, 0.788018f, 0.0573443f),
    vec3(0.908658f, 0.969634f, 0.0231941f),
    vec3(0.749901f, 0.74514f, 0.80697f),
    vec3(0.943968f, 0.634449f, 0.644398f),
    vec3(0.787225f, 0.353526f, 0.0561541f)};

vec3 rotateAxe(vec3 pts, vec3 axe, float angle)
{
    axe     = normalize(axe);
    float c = cos(angle);
    float s = sin(angle);
    mat3  M = mat3(vec3(axe.x * axe.x * (1 - c) + c, axe.x * axe.y * (1 - c) + axe.z * s, axe.x * axe.z * (1 - c) - axe.y * s),
                   vec3(axe.x * axe.y * (1 - c) - axe.z * s, axe.y * axe.y * (1 - c) + c, axe.y * axe.z * (1 - c) + axe.x * s),
                   vec3(axe.x * axe.z * (1 - c) + axe.y * s, axe.y * axe.z * (1 - c) - axe.x * s, axe.z * axe.z * (1 - c) + c));
    return pts * M;
}
vec3 prodVect(vec3 v, vec3 u)
{
    return vec3(v.y * u.z - v.z * u.y, v.z * u.x - v.x * u.z, v.x * u.y - v.y * u.x);
}
vec3 rotatePoints(vec3 p, vec3 pts)
{
    vec3  normalSphere = vec3(0.0, 0.000001, 1.0);
    vec3  axeRot       = prodVect(getNormal(p), normalSphere);
    float angle        = acos(dot(normalize(getNormal(p)), normalize(normalSphere)));
    pts                = rotateAxe(pts, axeRot, angle) + 0.05 * axeRot;
    return pts;
}

float ssao(vec3 p)
{
    float bl = 0;
    for (int i = 0; i < 32; i++) {
        if (is0_default_sdf(p + 1 * (rotatePoints(p, 0.3 * (BigRand[i] - vec3(0.5, 0.5, 0.0))))) > 0) {
            bl += 1;
        }
    }
    return bl / 32;
}
)";

static constexpr const char* inigo_ao_fct = R"(
vec2 hash2(float n) { return fract(sin(vec2(n, n + 1.0)) * vec2(43758.5453123, 22578.1459123)); }

float calcOcclusion(in vec3 pos, in vec3 nor, float ra)
{
    float occ = 0.0;
    for (int i = 0; i < 64; i++) {
        float h   = 0.01 + 2.0 * pow(float(i) / 63.0, 2.0);
        vec2  an  = hash2(ra + float(i) * 13.1) * vec2(3.14159, 6.2831);
        vec3  dir = vec3(sin(an.x) * sin(an.y), sin(an.x) * cos(an.y), cos(an.x));
        dir *= sign(dot(dir, nor));
        occ += clamp(5.0 * is0_default_sdf(pos + h * dir).x / h, -1.0, 1.0);
    }
    return clamp(occ / 64.0, 0.0, 1.0);
}
)";

static constexpr const char* edge_def = R"(
    
vec3 edgeColor = vec3(1.0,0.5,0.0);

float edges(vec3 ro, vec3 rd)
{
    float EDGE_WIDTH = 0.2;
    float edge       = 1.0;
    float lastdS     = is0_default_sdf(ro);
    float dO         = 0.;
    float steps      = 0.0;
    for (int i = 0; i < MAX_STEPS; i++) {
        steps += 1;
        vec3  p  = ro + rd * dO;
        float dS = is0_default_sdf(p);
        dO += dS;
        if (dO > MAX_DIST || abs(dS) < SURF_DIST) {
            break;
        } 
        if (lastdS < EDGE_WIDTH && dS > lastdS + 0.001) {
            edge = 0.0;
        }
        lastdS = dS;
        
    }
    return edge;
}

float edges2(vec3 ro, vec3 rd) {

    float EDGE_WIDTH = 0.1;
    float edge       = 1.0;
    float lastd     = is0_default_sdf(ro);
    float t = 0.;
    for (int i = 0; i < MAX_STEPS; i++) {
    	vec3 pos = ro + rd * t;
        float d = is0_main_sdf(pos);
        t += d;
        // If we are very close to the object, consider it as a hit and exit this loop
        if( t > MAX_DIST || abs(d) < SURF_DIST*0.99) break;
        if (lastd < EDGE_WIDTH+0.04*sqrt(t) && d > lastd + 0.001) {
            edge = 0.0;
        }
        lastd = d;
    }
    return edge;
}
)";

static constexpr const char*                            edge_fct = R"(

if(edges2(ro,rd) < 0.7){
    finalCol = edgeColor ;
}

)";
static std::vector<std::pair<std::string, std::string>> compute_sdf_identifiers(const Node& node, const NodeTemplate& node_template, const NodeTree& node_tree)
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

std::string full_shader_code(const NodeTree& node_tree, const std::vector<NodeTemplate>& node_templates)
{
    return ray_marcher_begin + std::string{default_sdf} + main_sdf(node_tree, node_templates) + ray_marcher_def + edge_def + inigo_ao_fct + poisson_pts + ray_marcher_end_1 + edge_fct + soft_shadows + poisson_ao + inigo_ao + ray_marcher_end_2;
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

std::string function_name(const FnNameParams& p)
{
    return std::string{p.node_template_name} + "_" + std::to_string(static_cast<unsigned int>(*p.node_id));
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