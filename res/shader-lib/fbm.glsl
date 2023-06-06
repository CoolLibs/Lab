// https://thebookofshaders.com/13/
// #include "_ROOT_FOLDER_/res/shader-lib/psrdnoise.glsl"

vec2 fbm(
    vec2 in_uv, float mask,
    float scale, float lacunarity, float gain, int octaves
)
{
    vec2 uv = in_uv;

    // Initial values
    float value     = 0.0;
    float amplitude = .5;
    float frequency = 0.;
    //
    // Loop of octaves
    for (int i = 0; i < octaves; i++)
    {
        value += amplitude * (snoise(uv * scale) * 0.5 + 0.5);
        uv *= lacunarity;
        amplitude *= gain;
    }
    return mix(in_uv, vec2(value), mask);
}