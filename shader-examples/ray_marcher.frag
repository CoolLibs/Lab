#version 430

in vec2       _uv;
uniform float _time;

#include "_COOL_RES_/shaders/camera.glsl"

#define MAX_STEPS 100
#define MAX_DIST  100.
#define SURF_DIST .001

float sdf(vec3 p)
{
    return length(mod(p + 5., vec3(10.)) - 5.) - 1.;
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

vec3 render(vec3 ro, vec3 rd)
{
    float d   = ray_march(ro, rd);
    vec3  col = vec3(1.000, 0.711, 0.949);

    if (d < MAX_DIST) {
        vec3 p = ro + rd * d;
        col    = normal(p) * 0.5 + 0.5;
        // vec3 n = normal(p);
        // vec3 r = reflect(rd, n);

        // float dif = dot(n, normalize(vec3(1,2,3))) * 0.5 + 0.5;
        // col = vec3(dif);
    }

    col = pow(col, vec3(.4545)); // gamma correction
    return col;
}

void main()
{
    vec3 ro = cool_ray_origin();
    vec3 rd = cool_ray_direction();

    gl_FragColor = vec4(render(ro, rd), 1.);
}