// https://www.shadertoy.com/view/stSGDW

vec2 distortion_flower(
    vec2 in_uv, float Effect_intensity,
    vec2 Center, float Scale, float Frequency, float Wave_compression, float subdivison
)
{
    vec2 uv = in_uv - Center;

    float r = sqrt(uv.x * uv.x + uv.y * uv.y) * Scale / 15.0;

    float a = 60.0 * sin(subdivison * atan(uv.x + uv.y / r, uv.y - uv.x / r));
    a += 500.0;
    a = a * (r / 50.0);
    a = 200000.0 * sin(a * 6.0) * (r / 30.0) * Frequency;

    return mix(in_uv, uv * vec2(cos(a / 20.0), cos(a / 20. * Wave_compression)), Effect_intensity);
}