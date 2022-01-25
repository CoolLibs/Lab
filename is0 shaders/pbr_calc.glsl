vec3 Fresnel1Term(vec3 specularCol, float vdoth)
{
    vec3 fresnel = specularCol + (1. - specularCol) * pow((1. - vdoth), 5.);
    return fresnel;
}

float DistributionTerm(float roughtness, float ndoth)
{
    float r2 = roughtness * roughtness;
    float d  = (ndoth * r2 - ndoth) * ndoth + 1.0;
    return r2 / (d * d * 3.1415);
}

float VisibilityTerm(float roughtness, float ndotv, float ndot1)
{
    float r2 = roughtness * roughtness;
    float gv = ndot1 * sqrt(ndotv * (ndotv - ndotv * r2) + r2);
    float g1 = ndotv * sqrt(ndot1 * (ndot1 - ndot1 * r2) + r2);
    return 0.5 / max(gv + g1, 0.00001);
}

vec3 EnvBRDFApprox(vec3 specularCol, float roughtness, float ndotv)
{
    const vec4 c0   = vec4(-1, -0.0275, -0.572, 0.022);
    const vec4 c1   = vec4(1, 0.0425, 1.04, -0.04);
    vec4       r    = roughtness * c0 + c1;
    float      a004 = min(r.x * r.x, exp2(-9.28 * ndotv)) * r.x + r.y;
    vec2       AB   = vec2(-1.04, 1.04) * a004 + r.zw;
    return specularCol * AB.x + AB.y;
}

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
    float refraction_index;
    float refraction_abberation;
    float refraction_density;

    //Glow
    float glow_strength;
    vec3  glow_color;
};  