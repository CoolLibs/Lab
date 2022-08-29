
// https://www.shadertoy.com/view/lsXSWl

// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"

float rng2(vec2 seed, float time_mod)
{
    return fract(sin(dot(seed * floor(time_mod * 12.), vec2(127.1, 311.7))) * 43758.5453123);
}

float rng(float seed, float time_mod)
{
    return rng2(vec2(seed, 1.0), time_mod);
}

vec3 chromatic_aberration_glitch(
    vec2 in_uv, float Effect_intensity,
    float Tint, float Darkness, vec2 BlockS, vec2 BlockL, float Time_mod
)
{
    vec2 blockS = floor(in_uv * BlockS);
    vec2 blockL = floor(in_uv * BlockL);

    vec3 noise = (vec3(Tint, 1. - Tint, Tint / 2. + 0.5) - vec3(2.0)) * Darkness;

    float lineNoise = pow(rng2(blockS, Time_mod), 8.) * pow(rng2(blockL, Time_mod), 3.) - pow(rng(7.2341, Time_mod), 17.) * 2.;

    vec3 col1 = image(in_uv);
    vec3 col2 = image(in_uv + vec2(lineNoise * Effect_intensity * rng(5., Time_mod), 0.));
    vec3 col3 = image(in_uv - vec2(lineNoise * Effect_intensity * rng(31, Time_mod), 0.));

    vec3 color = vec3(col1.r, col2.g, col3.b) + noise;
    return color;
}