
// https://www.shadertoy.com/view/lsXSWl

// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"

float rng2(vec2 seed, float time)
{
    return fract(sin(dot(seed * floor(time * 12.), vec2(127.1, 311.7))) * 43758.5453123);
}

float rng(float seed, float time)
{
    return rng2(vec2(seed, 1.0), time);
}

vec3 glitch(
    vec2 in_uv, float effect_intensity,
    vec2 squares_size, float time
)
{
    const vec2 squares_sizeL = vec2(8., 4.);

    vec2 blockS = floor(in_uv * squares_size);
    vec2 blockL = floor(in_uv * squares_sizeL);

    float lineNoise = pow(rng2(blockS, time), 8.) * pow(rng2(blockL, time), 3.) - pow(rng(7.2341, time), 17.) * 2.;

    vec3 col1 = image(in_uv);
    vec3 col2 = image(in_uv + vec2(lineNoise * effect_intensity * rng(5., time), 0.));
    vec3 col3 = image(in_uv - vec2(lineNoise * effect_intensity * rng(31, time), 0.));

    vec3 color = vec3(col1.r, col2.g, col3.b);
    return color;
}