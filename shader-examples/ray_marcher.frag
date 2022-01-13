#version 430

in vec2       _uv;
uniform float _time;
out vec4      out_Color;

#include "_COOL_RES_/shaders/camera.glsl"

#define MAX_STEPS 100
#define MAX_DIST  100.
#define SURF_DIST .001

float sdf(vec3 p)
{
    // ivec3 idx = ivec3(floor(p / 8) - 0.5);
    // if (idx.x % 2 == 0) {
    //     p += _time;
    // }
    // else {
    //     p -= _time;
    // }
    // vec3 p1 = (fract(p / 8) - 0.5) * 8;
    // vec3 q  = abs(p1) - 1.0;
    // return length(max(q, 0.)) + min(max(q.x, max(q.y, q.z)), 0.);
    return length(mod(p + 5., vec3(10.)) - 5.) - 2.;
}

float ray_march(vec3 ro, vec3 rd)
{
    float dO = 0.;

    for (int i = 0; i < MAX_STEPS; i++) {
        vec3  p  = ro + rd * dO;
        float dS = sdf(p);
        dO += dS;
        if (dO > MAX_DIST || abs(dS) < SURF_DIST) {
            break;
        }
    }

    return dO;
}

vec3 normal(vec3 p)
{
    float      d = sdf(p);
    const vec2 e = vec2(.001, 0);

    vec3 n = d - vec3(
                     sdf(p - e.xyy),
                     sdf(p - e.yxy),
                     sdf(p - e.yyx));

    return normalize(n);
}

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

vec3 render(vec3 ro, vec3 rd)
{
    float d     = ray_march(ro, rd);
    vec3  color = vec3(1.000, 0.711, 0.949);
    vec3  p     = ro + rd * d;
    vec3  n     = normal(p);
    vec3  r     = reflect(rd, n);
    vec4  rebond;

    vec3  ld            = normalize(vec3(2.));
    vec3  lc            = vec3(1.);
    vec3  baseColor     = vec3(.5);
    vec3  diffuseColor  = baseColor;
    vec3  specularColor = vec3(0.02);
    float roughtnessE   = 0.1;
    float roughtnessL   = max(.01, roughtnessE);

    if (d < MAX_DIST) {
        vec3 diffuse  = vec3(0.);
        vec3 specular = vec3(0.);

        vec3  halfVec    = normalize(ro + ld);
        float vdoth      = clamp(dot(ro, halfVec), 0., 1.);
        float ndoth      = clamp(dot(n, halfVec), 0., 1.);
        float ndotv      = clamp(dot(n, ro), 0., 1.);
        float ndot1      = clamp(dot(n, ld), 0., 1.);
        vec3  envSpecCol = EnvBRDFApprox(specularColor, roughtnessE, ndotv);

        diffuse += diffuseColor;
        specular += envSpecCol;
        diffuse += diffuseColor * lc * clamp(dot(n, ld), 0., 1.);
        vec3  lightF = Fresnel1Term(specularColor, vdoth);
        float lightD = DistributionTerm(roughtnessL, ndoth);
        float lightV = VisibilityTerm(roughtnessL, ndotv, ndot1);
        specular += lc * lightF * (lightD * lightV * 3.1415 * ndot1);

        color = diffuse + specular;
    }
    else {
        float planeT = -(ro.y + 1.2) / rd.y;
        if (planeT > 0.0) {
            vec3  p      = ro + planeT * rd;
            float radius = .7;
            color *= 0.7 + 0.3 * smoothstep(0.0, 1.0, clamp(length(p + vec3(0.0, 1.0, -.05)) - radius, 0., 1.));
        }
    }
    color = pow(color, vec3(.4545)); // gamma correction
    return color;
}

void main()
{
    vec3 ro = cool_ray_origin();
    vec3 rd = cool_ray_direction();

    out_Color = vec4(render(ro, rd), 1.);
}
