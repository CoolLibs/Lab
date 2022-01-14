#include "RenderEffect_Reflection.h"

std::string code_gen_reflection_parameters(const RenderEffect_Reflection& reflection)
{
    return "const float IOR = " +
           std::to_string(*reflection.strenght) +
           ";\n\n";
};

std::string code_gen_reflection()
{
    return R"(
        vec3 refletOut = reflect(rd, normal);
        vec3 refractionIn = refract(rd,normal,1./IOR);

        vec3 pEnter = p - normal * SURF_DIST * 3.;
        RayMarchRes dIn = rayMarching(pEnter, refractionIn, INVERSED_SDF); // Inside
        float d2 = dIn.dist;

        vec3 pExit = pEnter + refractionIn * d2;
        vec3 normExit = -getNormal(pExit);
        vec3 reflectText = vec3(0);
        vec3 refractionOut = vec3(0);
        float abb = .01;
        //Red
        refractionOut = refract(refractionIn, normExit, IOR-abb);
        if(dot(refractionOut, refractionOut) == 0.){
            refractionOut = reflect(refractionIn, normExit);
        }
        reflectText.r = refractionOut.r;
        //Green
        refractionOut = refract(refractionIn, normExit, IOR);
        if(dot(refractionOut, refractionOut) == 0.){
            refractionOut = reflect(refractionIn, normExit);
        }
        reflectText.g = refractionOut.g;
        //Blue
        refractionOut = refract(refractionIn, normExit, IOR+abb);
        if(dot(refractionOut, refractionOut) == 0.){
            refractionOut = reflect(refractionIn, normExit);
        }
        reflectText.b = refractionOut.b;
        
        float dens = .1;
        float optDist = exp(-d2 * dens);
        reflectText= reflectText * optDist;
        float fresnelRefl = pow(1. + dot(rd, normal), 5.);

        finalCol += mix(reflectText, refletOut, fresnelRefl);
    )";
};

bool reflection_imgui(RenderEffect_Reflection& reflection)
{
    ImGui::Text("Reflection");
    bool has_changed = reflection.strenght.imgui();
    has_changed |= ImGui::Checkbox("Reflection Active", &reflection.is_active);
    return has_changed;
};