#version 430

layout(location = 0) in vec2       _uv;
uniform float _time;
out vec4      out_Color;
#include "_COOL_RES_/shaders/camera.glsl"

#define MAX_STEPS 100
#define MAX_DIST  100.
#define SURF_DIST .001

// BEGIN DYNAMIC PARAMS

uniform vec3 col;

// END DYNAMIC PARAMS

float sdCapsule(vec3 p, vec3 a, vec3 b, float r)
{
    vec3  pa = p - a, ba = b - a;
    float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
    return length(pa - ba * h) - r;
}

vec2 sdf(vec3 p)
{
    float dX = sdCapsule(p, vec3(0, 0, 0), vec3(10, 0, 0), 0.1);
    float dY = sdCapsule(p, vec3(0, 0, 0), vec3(0, 10, 0), 0.1);
    float dZ = sdCapsule(p, vec3(0, 0, 0), vec3(0, 0, 10), 0.1);

    float d = min(min(dX, dY), dZ);
    return vec2(
        d,
        d == dX ? 0 : d == dY ? 1
                              : 2);
}

float ray_march(vec3 ro, vec3 rd)
{
    float dO = 0.;

    for (int i = 0; i < MAX_STEPS; i++) {
        vec3  p  = ro + rd * dO;
        float dS = sdf(p).x;
        dO += dS;
        if (dO > MAX_DIST || abs(dS) < SURF_DIST) {
            break;
        }
    }

    return dO;
}

vec3 normal(vec3 p)
{
    float      d = sdf(p).x;
    const vec2 e = vec2(.001, 0);

    vec3 n = d - vec3(
                     sdf(p - e.xyy).x,
                     sdf(p - e.yxy).x,
                     sdf(p - e.yyx).x);

    return normalize(n);
}

vec3 render(vec3 ro, vec3 rd)
{
    float d   = ray_march(ro, rd);
    vec3  col = col;

    if (d < MAX_DIST) {
        vec3  p  = ro + rd * d;
        float id = sdf(p).y;
        col =
            id < 0.5 ? vec3(1, 0, 0) : id < 1.5 ? vec3(0, 1, 0)
                                                : vec3(0, 0, 1);

        float dif = dot(normal(p), normalize(vec3(1, 2, 3))) * 0.5 + 0.5;
        col *= dif;
    }

    col = pow(col, vec3(.4545)); // gamma correction
    return col;
}

void main()
{
    vec3 ro = cool_ray_origin();
    vec3 rd = cool_ray_direction();

    out_Color = vec4(render(ro, rd), 1.);
}