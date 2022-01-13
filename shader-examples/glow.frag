#version 430

layout(location = 0) in vec2       _uv;
uniform float _time;
out vec4      out_Color;

#include "_COOL_RES_/shaders/camera.glsl"

#define MAX_STEPS 100
#define MAX_DIST  100.
#define SURF_DIST .001

// BEGIN DYNAMIC PARAMS

uniform float glow_strength;
uniform vec3 glow_color;

// END DYNAMIC PARAMS

float sdf(vec3 p)
{
    return length(p) - 1.;
}

struct RayMarchRes {
    float dist;
    float pas;
};

RayMarchRes ray_march(vec3 ro, vec3 rd)
{
    float dO = 0.;
    int i = 0;

    for (i; i < MAX_STEPS; i++) {
        vec3  p  = ro + rd * dO;
        float dS = sdf(p);
        dO += dS;
        if (dO > MAX_DIST || abs(dS) < SURF_DIST) {
            break;
        }
    }

    return RayMarchRes(dO,i);
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

vec3 render(vec3 ro, vec3 rd)
{
    RayMarchRes res = ray_march(ro, rd);
    vec3  col = vec3(.000, 0.0711, 0.0949);
    float d = res.dist;
    float pas = res.pas;

    if (d < MAX_DIST) {
        vec3 p = ro + rd * d;
        vec3 norm = normal(p); 
        col    = norm * 0.5 + 0.5;
        // vec3 n = normal(p);
        // vec3 r = reflect(rd, n);

        // float dif = dot(n, normalize(vec3(1,2,3))) * 0.5 + 0.5;
        // col = vec3(dif);
    }
    float glow = pow(pas*glow_strength,2);
    col += glow * glow_color;
    col = pow(col, vec3(.4545)); // gamma correction
    return col;
}

void main()
{
    vec3 ro = cool_ray_origin();
    vec3 rd = cool_ray_direction();

    out_Color = vec4(render(ro, rd), 1.);
}