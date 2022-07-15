#version 410

// https://www.shadertoy.com/view/WdKXWt
/*
Shader coded live on twitch (https://www.twitch.tv/nusan_fx)
The shader was made using Bonzomatic.
You can find the original shader here: http://lezanu.fr/LiveCode/DropOfDistortion.glsl
*/

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform float time_mod;
uniform float drops_size;     // default 0.2 min = .001 max = 1.5
uniform float distortion;     // default 0.7 min = 0 max = 1
uniform float size;           // default 1 min = 0.001 max = 1.5
uniform float border_color_r; // default 1 min = -1 max = 1
uniform float border_color_g; // default 1 min = -1 max = 1
uniform float border_color_b; // default 1 min = -1 max = 1

// END DYNAMIC PARAMS

float time;

float sph(vec3 p, float r)
{
    return length(p) - r;
}

float cyl(vec2 p, float r)
{
    return length(p) - r;
}

mat2 rot(float a)
{
    float ca = cos(a);
    float sa = sin(a);
    return mat2(ca, sa, -sa, ca);
}

float smin(float a, float b, float h)
{
    float k = clamp((a - b) / h * 0.5 + 0.5, 0.0, 1.0);
    return mix(a, b, k) - k * (1.0 - k) * h;
}

vec3 smin(vec3 a, vec3 b, float h)
{
    vec3 k = clamp((a - b) / h * 0.5 + 0.5, 0.0, 1.0);
    return mix(a, b, k) - k * (1.0 - k) * h;
}

float tick(float t, float d)
{
    t /= d;
    float g = fract(t);
    g       = smoothstep(0.0, 1.0, g);
    g       = pow(g, 10.0);
    return (floor(t) + g) * d;
}

// Kaleidoscopic iterated function system
vec3 kifs(vec3 p, float t)
{
    for (int i = 0; i < 3; ++i)
    {
        float t1 = tick(t + float(i), 0.4 + float(i) * 0.1) + t * 0.3;
        p.xz *= rot(t1);
        p.yz *= rot(t1 * 0.7);

        // p = abs(p);
        //  symmetry on all axis with a smooth transition
        p = smin(p, -p, -1.0);
        p -= vec3(1.2, 0.5, 1.6);
    }

    return p;
}

float map(vec3 p)
{
    vec3 bp = p;

    float t0 = time * 0.7;
    float t1 = tick(t0, 1.3) * 0.3 + t0 * 0.1;
    p.xy *= rot(t1);
    p.yz *= rot(t1 * 1.3);

    vec3 p2 = kifs(p, t0 * 0.3);
    vec3 p3 = kifs(p + vec3(1, 0, 0.3), t0 * 0.2);

    float d  = sph(p2, 1.0);
    float d2 = cyl(p3.xz, 1.1);

    // substract d2 to d1 with a smooth transition
    d = smin(d, -d2, -1.0);

    // remove whats above and below a depth plane to keep only a slice
    d = smin(d, -bp.z - 4.0, -0.3);
    d = smin(d, bp.z - 4.0, -0.3);

    return d;
}

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    time = _time;

    vec2 uv = _uv;
    uv -= 0.5;

    vec3 s = vec3(0, 0, -17);
    vec3 r = normalize(vec3(uv, size));

    vec3  p      = s;
    float at     = 0.0;
    bool  inside = false;
    // main raymarching loop
    for (int i = 0; i < 100; ++i)
    {
        float d = map(p);
        if (d < drops_size)
        {
            inside = true;
            break;
        }
        if (d > 100.0)
        {
            break;
        }
        p += r * d;
        at += 0.9 / (3.0 + abs(d));
    }

    // // if we hit a surface
    if (inside)
    {
        vec2 off = vec2(0.01, 0);
        vec3 n   = normalize(map(p) - vec3(map(p - off.xyy), map(p - off.yxy), map(p - off.yyx)));
        // refract the ray direction
        r = refract(r, n, distortion);
    }

    float depth = length(p - s);

    vec2 uv2 = p.xy * size / (depth * r.z) + vec2(0.5, .5);

    vec3 col = image(uv2).rgb;

    vec3 border_color = vec3(border_color_r, border_color_g, border_color_b) + vec3(sin(_time * 4) * 0.05);
    col += at * border_color;
    col *= pow(max(0.0, 1.3 - length(uv)), 0.8);

    out_Color = vec4(col, 1);
}