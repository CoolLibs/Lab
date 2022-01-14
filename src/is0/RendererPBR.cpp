#include "RendererPBR.h"
#include <Cool/String/String.h>
#include <numeric>
#include <sstream>
#include "CodeGen.h"

namespace CodeGen {

std::string clampCodeGen(const std::string& v1, const std::string& v2)
{
    std::stringstream clampDefinition;
    clampDefinition << "clamp(dot(" << v1 << "," << v2 << "), 0., 1.);\n";
    return clampDefinition.str();
}

std::string lightPropCodeGen(const LightProperties& l)
{
    std::stringstream lightDefinition;

    lightDefinition << "    vec3 ld = ";
    lightDefinition << glm::to_string(*l.lightDirection) << ";\n";
    lightDefinition << "    vec3 lc = ";
    lightDefinition << glm::to_string(*l.lightColor) << " * " << std::to_string(*l.lightIntensity) << ";\n";

    return lightDefinition.str();
}

std::string MaterialPropCodeGen(const MaterialProperties& m)
{
    std::stringstream materialDefinition;

    materialDefinition << "    vec3 baseColor = ";
    materialDefinition << glm::to_string(*m.MaterialBaseColor) << ";\n";
    materialDefinition << "    vec3 DiffuseColor = ";
    materialDefinition << glm::to_string(*m.MaterialDiffuseColor) << ";\n";
    materialDefinition << "    vec3 SpecularColor = ";
    materialDefinition << glm::to_string(*m.MaterialSpecularColor) << ";\n";
    materialDefinition << "    float roughtness = " << std::to_string(*m.roughtness) << ";\n";

    return materialDefinition.str();
}

std::string PBRRendererCodeGen(const LightProperties& light, const MaterialProperties& material)
{
    std::stringstream rendererDefinition;

    rendererDefinition << "\n\nfloat rayMarching(vec3 ro, vec3 rd) {\n    float t = 0.;\n\n    for (int i = 0; i < MAX_STEPS; i++) {\n    	vec3 pos = ro + rd * t;\n        float d = is0_main_sdf(pos);\n        t += d;\n        // If we are very close to the object, consider it as a hit and exit this loop\n        if( t > MAX_DIST || abs(d) < SURF_DIST*0.99) break;\n    }\n    return t;\n}";
    rendererDefinition << "\n\nvec3 getNormal(vec3 p) {\n    const float h = NORMAL_DELTA;\n	const vec2 k = vec2(1., -1.);\n    return normalize( k.xyy * is0_main_sdf( p + k.xyy*h ) + \n                      k.yyx * is0_main_sdf( p + k.yyx*h ) + \n                      k.yxy * is0_main_sdf( p + k.yxy*h ) + \n                      k.xxx * is0_main_sdf( p + k.xxx*h ) );\n}\n\n";
    rendererDefinition << "\n\n//Based on : https://www.shadertoy.com/view/4sSfzK\n";
    rendererDefinition << "vec3 render(vec3 ro, vec3 rd)\n{\n    float d = rayMarching(ro,rd);\n    vec3 color = vec3(0.020,0.711,0.949);\n    vec3 p = ro + rd * d;\n    vec3 n = getNormal(p);\n    vec3 r = reflect(rd,n);\n";
    rendererDefinition << lightPropCodeGen(light);
    rendererDefinition << MaterialPropCodeGen(material);
    rendererDefinition << "    if (d < MAX_DIST) \n{ \n        vec3 diffuse = vec3(0.);\n        vec3 specular = vec3(0.);\n\n        vec3 halfVec = normalize(ro + ld);\n        float vdoth = ";
    rendererDefinition << clampCodeGen("ro", "halfVec");
    rendererDefinition << "        float ndoth = " << clampCodeGen("n", "halfVec");
    rendererDefinition << "        float ndotv = " << clampCodeGen("n", "ro");
    rendererDefinition << "        float ndot1 = " << clampCodeGen("n", "ld");
    rendererDefinition << "        vec3 envSpecCol = EnvBRDFApprox(SpecularColor, roughtness, ndotv);\n\n        diffuse += DiffuseColor;\n        //specular += envSpecCol;\n        diffuse += DiffuseColor * lc * " << clampCodeGen("n", "ld");
    rendererDefinition << "        vec3 lightF = Fresnel1Term(SpecularColor, vdoth);\n        float lightD = DistributionTerm(roughtness, ndoth);\n        float lightV = VisibilityTerm(roughtness, ndotv, ndot1);\n";
    rendererDefinition << "        specular += lc * lightF * (lightD * lightV * 3.1415 * ndot1);\n\n        color = baseColor + diffuse + specular;\n";
    rendererDefinition << "    }\n    color = pow(color, vec3(.4545)); //gamma correction\n    return color;\n}";

    return rendererDefinition.str();
}

} // namespace CodeGen