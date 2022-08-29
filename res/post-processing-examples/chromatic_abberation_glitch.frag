#version 410

// https://www.shadertoy.com/view/lsXSWl

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"

INPUT float Time_mod;

INPUT float Effect_intensity; // default 1. min 0. max 1.

float rng2(vec2 seed, float time_mod)
{
    return fract(sin(dot(seed * floor(time_mod * 12.), vec2(127.1, 311.7))) * 43758.5453123);
}

float rng(float seed, float time_mod)
{
    return rng2(vec2(seed, 1.0), time_mod);
}

void main()
{
    vec2 uv = _uv;

    vec2 blockS = floor(uv * vec2(24., 9.));
    vec2 blockL = floor(uv * vec2(8., 4.));

    float r     = rng2(uv, Time_mod);
    vec3  noise = (vec3(r, 1. - r, r / 2. + 0.5) * 1.0 - 2.0) * 0.08;

    float lineNoise = pow(rng2(blockS, Time_mod), 8.) * pow(rng2(blockL, Time_mod), 3.) - pow(rng(7.2341, Time_mod), 17.) * 2.;

    vec3 col1 = image(uv);
    vec3 col2 = image(uv + vec2(lineNoise * 0.05 * rng(5., Time_mod), 0));
    vec3 col3 = image(uv - vec2(lineNoise * 0.05 * rng(31, Time_mod), 0));

    vec3 color = vec3(col1.r, col2.g, col3.b) + noise;
    out_Color  = vec4(color, 1.);
}
