// https://thebookofshaders.com/13/
// #include "_ROOT_FOLDER_/shader-lib/psrdnoise.glsl"

vec2 fbm(
    float Effect_intensity, in vec2 uv,
    float Scale, float Lacunarity, float Gain, int Octaves
)
{
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
    return mix(uv, vec2(value), Effect_intensity);
}