#version 410

// https://www.shadertoy.com/view/Ws3BzS

// #include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"

INPUT float time_mod;
INPUT float thres; // default 0.7 min = 0 max = 2
INPUT float res_x;
INPUT float res_y;
const float steps = 4; // default 4

float random(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float plasma(vec2 uv)
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

void main()
{
    vec2 iResolution = vec2(res_x, res_y);
    vec2 fragCoord   = _uv * iResolution;
    int  xModif      = 0;
    int  offs        = int(min(steps, int(fragCoord.x)));
    for (int x = 0; x < int(fragCoord.x); x += offs)
    {
        offs       = int(min(steps, int(fragCoord.x) - x));
        float psin = sin(plasma(vec2(x, fragCoord.y) / iResolution.xy));
        float pcos = cos(plasma(vec2(fragCoord.y, x) / iResolution.xy));

        if (abs(psin - pcos) > thres)
        {
            xModif += offs;
        }
    }

    vec2     uvModif = vec2(xModif, fragCoord.y) / iResolution.xy;
    RgbColor col     = texture(_image, uvModif).rgb;
    out_Color        = vec4(col, 1.);
}