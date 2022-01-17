#include "RendererPBR.h"
#include <Cool/String/String.h>
#include <numeric>
#include <sstream>
#include "CodeGen.h"

namespace CodeGen {

std::string light_prop_codegen(const LightProperties& l)
{
    std::stringstream lightDefinition;

    lightDefinition << "    vec3 ld = ";
    lightDefinition << glm::to_string(*l.lightDirection) << ";\n";
    lightDefinition << "    vec3 lc = ";
    lightDefinition << glm::to_string(*l.lightColor) << " * " << std::to_string(*l.lightIntensity) << ";\n";
    lightDefinition << "    float rebondIntensity = ";
    lightDefinition << std::to_string(*l.RebondIntensity) << ";\n";

    return lightDefinition.str();
}

std::string material_prop_codegen(const MaterialProperties& m)
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

std::string pbr_renderer_codegen(const LightProperties& light, const MaterialProperties& material)
{
    std::stringstream rendererDefinition;

    rendererDefinition << R"(
        
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
        };
        )";

    rendererDefinition << R"(
        
        //Based on : https://www.shadertoy.com/view/4sSfzK\n";
        vec3 rebond(vec3 ro, vec3 rd)
        {
            float d = rayMarching(ro,rd);
            vec3 color = vec3(0.020,0.711,0.949);
            vec3 p = ro + rd * d;
            vec3 n = getNormal(p);
            vec3 r = reflect(rd,n);
            )";
    rendererDefinition << light_prop_codegen(light);
    rendererDefinition << material_prop_codegen(material);
    rendererDefinition << R"(    
        if (d < MAX_DIST) 
        {
            vec3 diffuse = vec3(0.);
            vec3 specular = vec3(0.);
            vec3 halfVec = normalize(ro + ld);
            float vdoth = )";
    rendererDefinition << "clamp(dot(ro, halfVec), 0., 1.);\n";
    rendererDefinition << "        float ndoth = clamp(dot( n, halfVec), 0., 1.);\n";
    rendererDefinition << "        float ndotv = clamp(dot( n, ro ), 0., 1.);\n";
    rendererDefinition << "        float ndot1 = clamp(dot( n, ld), 0., 1.);\n";
    rendererDefinition << R"(        
            diffuse += DiffuseColor;
            //specular += envSpecCol;
            diffuse += DiffuseColor * lc * clamp(dot(n, ld), 0., 1.);
            )";
    rendererDefinition << R"(        vec3 lightF = Fresnel1Term(SpecularColor, vdoth);
            float lightD = DistributionTerm(roughtness, ndoth);
            float lightV = VisibilityTerm(roughtness, ndotv, ndot1);
            specular += lc * lightF * (lightD * lightV * 3.1415 * ndot1);
            
            color = baseColor + diffuse + specular;
        }
        return color;
    })";

    rendererDefinition << R"(
    vec3 render(vec3 ro, vec3 rd)
    {
        float d = rayMarching(ro,rd);
        vec3 color = vec3(0.020,0.711,0.949);
        vec3 p = ro + rd * d;
        vec3 n = getNormal(p);
        vec3 r = reflect(rd,n);
    )";
    rendererDefinition << light_prop_codegen(light);
    rendererDefinition << material_prop_codegen(material);
    rendererDefinition << R"(    if (d < MAX_DIST) 
        { 
            vec3 diffuse = vec3(0.);
            vec3 specular = vec3(0.);
            
            vec3 halfVec = normalize(ro + ld);
            float vdoth = clamp(dot(ro, halfVec), 0., 1.);)";
    rendererDefinition << "        float ndoth = clamp(dot( n, halfVec), 0., 1.);\n";
    rendererDefinition << "        float ndotv = clamp(dot( n, ro ), 0., 1.);\n";
    rendererDefinition << "        float ndot1 = clamp(dot( n, ld), 0., 1.);\n";
    rendererDefinition << R"(        vec3 envSpecCol = EnvBRDFApprox(SpecularColor, roughtness, ndotv);
    
        diffuse += DiffuseColor;
        //specular += envSpecCol;
        diffuse += DiffuseColor * lc * clamp(dot(n, ld), 0., 1.);
        )";
    rendererDefinition << R"(        vec3 lightF = Fresnel1Term(SpecularColor, vdoth);
        float lightD = DistributionTerm(roughtness, ndoth);
        float lightV = VisibilityTerm(roughtness, ndotv, ndot1);
        specular += lc * lightF * (lightD * lightV * 3.1415 * ndot1);
        
        color = normalize(baseColor) + diffuse + specular;
        color += rebondIntensity * normalize(rebond(p + 0.1 * r, r));
    }
    color = pow(color, vec3(.4545)); //gamma correction
    return color;
    })";

    return rendererDefinition.str();
}

bool material_imgui_window(MaterialProperties& m)
{
    ImGui::Begin("Material");
    ImGui::Text("Base Color");
    bool has_changed = m.MaterialBaseColor.imgui();
    ImGui::Text("Diffuse Color");
    has_changed |= m.MaterialDiffuseColor.imgui();
    ImGui::Text("Specular Color");
    has_changed |= m.MaterialSpecularColor.imgui();
    has_changed |= m.roughtness.imgui();
    ImGui::End();

    return has_changed;
}

bool light_imgui_window(LightProperties& l)
{
    ImGui::Begin("Lighting");
    bool has_changed = l.lightColor.imgui();
    has_changed |= l.lightDirection.imgui();
    has_changed |= l.lightIntensity.imgui();
    has_changed |= l.RebondIntensity.imgui();
    ImGui::End();

    return has_changed;
}

} // namespace CodeGen