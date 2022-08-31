// https://thebookofshaders.com/13/
// #include "_ROOT_FOLDER_/res/shader-lib/psrdnoise.glsl"

vec2 fbm(
    vec2 in_uv, float Effect_intensity,
    float Scale, float Lacunarity, float Gain, int Octaves
)
{
    vec2 uv = in_uv;

    // Initial values
    float value     = 0.0;
    float amplitude = .5;
    float frequency = 0.;
    //
    // Loop of octaves
    for (int i = 0; i < Octaves; i++)
    {
        value += amplitude * (snoise(uv * Scale) * 0.5 + 0.5);
        uv *= Lacunarity;
        amplitude *= Gain;
    }
    return mix(in_uv, vec2(value), Effect_intensity);
}