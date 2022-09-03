// https://www.shadertoy.com/view/Ws3BzS

float random(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float plasma(vec2 uv, float Continuous_seed)
{
    float v = 0.0;
    float k = 9.0;
    vec2  c = uv * k - k / 2.0;

    v += sin((c.x + Continuous_seed));
    v += sin((c.y + Continuous_seed) / 2.0);

    v += sin((c.x + c.y + Continuous_seed) / 3.0);

    c += k / 2.0 * vec2(sin(Continuous_seed / 3.2), cos(Continuous_seed / 2.7));

    v += sin(sqrt(c.x * c.x + c.y * c.y + 1.0) + Continuous_seed);

    v = v / 2.0;

    return v;
}

int offset(int x, int Steps)
{
    return int(max(1, min(Steps, x)));
}

vec2 distortion_line(
    vec2 in_uv, float Effect_intensity,
    float Continuous_seed, float Threshold, vec2 Distortion, int Steps
)
{
    vec2 uv     = in_uv * Distortion;
    int  xModif = 0;
    int  offs   = offset(int(uv.x), Steps);
    for (int x = 0; x < int(uv.x); x += offs)
    {
        offs       = offset(int(uv.x) - x, Steps);
        float psin = sin(plasma(vec2(x, uv.y) / Distortion.xy, Continuous_seed));
        float pcos = cos(plasma(vec2(uv.y, x) / Distortion.xy, Continuous_seed));

        if (abs(psin - pcos) > Threshold)
        {
            xModif += offs;
        }
    }

    vec2 uvModif = vec2(xModif, uv.y) / Distortion.xy;
    return mix(in_uv, uvModif, Effect_intensity);
}