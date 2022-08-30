// https://www.shadertoy.com/view/Ws3BzS

float random(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float plasma(vec2 uv, float time_mod)
{
    float v = 0.0;
    float k = 9.0;
    vec2  c = uv * k - k / 2.0;

    v += sin((c.x + time_mod));
    v += sin((c.y + time_mod) / 2.0);

    v += sin((c.x + c.y + time_mod) / 3.0);

    c += k / 2.0 * vec2(sin(time_mod / 3.2), cos(time_mod / 2.7));

    v += sin(sqrt(c.x * c.x + c.y * c.y + 1.0) + time_mod);

    v = v / 2.0;

    return v;
}

vec2 distortion_line(
    vec2 in_uv, float Effect_intensity,
    float Time_mod, float Threshold, vec2 Distortion, int Steps
)
{
    vec2 uv     = in_uv * Distortion;
    int  xModif = 0;
    int  offs   = int(min(Steps, int(uv.x)));
    for (int x = 0; x < int(uv.x); x += offs)
    {
        offs       = int(min(Steps, int(uv.x) - x));
        float psin = sin(plasma(vec2(x, uv.y) / Distortion.xy, Time_mod));
        float pcos = cos(plasma(vec2(uv.y, x) / Distortion.xy, Time_mod));

        if (abs(psin - pcos) > Threshold)
        {
            xModif += offs;
        }
    }

    vec2 uvModif = vec2(xModif, uv.y) / Distortion.xy;
    return mix(in_uv, uvModif, Effect_intensity);
}