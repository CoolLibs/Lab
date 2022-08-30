// https://www.shadertoy.com/view/7tGGDW

float thc(float a, float b)
{
    return tanh(a * cos(b)) / tanh(a);
}

float ths(float a, float b)
{
    return tanh(a * sin(b)) / tanh(a);
}

vec2 thc(float a, vec2 b)
{
    return tanh(a * cos(b)) / tanh(a);
}

vec2 ths(float a, vec2 b)
{
    return tanh(a * sin(b)) / tanh(a);
}

vec3 pal(in float t, in vec3 a, in vec3 b, in vec3 c, in vec3 d)
{
    return a + b * cos(6.28318 * (c * t + d));
}

float h21(vec2 a)
{
    return fract(sin(dot(a.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

float mlength(vec2 uv)
{
    return max(abs(uv.x), abs(uv.y));
}

vec2 distortion_log_polar(
    vec2 in_uv, float Effect_intensity,
    vec2 Center, float Scale1, float Scale2, Angle Angle_in_turns, float Flowerness, int Subdivision
)
{
    vec2 uv = in_uv - Center;

    uv /= cosh(length(uv) * Scale1);

    float a = atan(uv.y, uv.x);
    float r = length(uv);

    uv = mix(in_uv, vec2(thc(2., a), thc(2., Flowerness * thc(3., Subdivision * a + Angle_in_turns * TAU * 6.) + log(r))), Effect_intensity);

    float sc = 0.; // + 1.5 * (1. + Flowerness * thc(3., log(r))) * thc(1., Scale2 * r);
    // sc = tan(sc);
    // vec2 ipos = floor(sc * uv) + 0.5;
    vec2 fpos = fract(sc * uv) - Center;

    float d = length(fpos);
    float v = 0.5 + 0.5 * cos(6. * uv.y);
    float k = 0.4 * (1. - v);                                         // anything * (1.-v) works
    float s = smoothstep(-k, k, v - d) - smoothstep(-k, k, 0.03 - d); // was 0.1

    // s * X + ..., higher X = more distortion
    // s = cos(s * 4. + 2. * a + 8. * r + Time_mod);

    return mix(_uv, s * pal(uv.y + s, vec3(1.), vec3(1.), vec3(1.), vec3(0., uv.y, 2. * uv.y)).xy * Angle_in_turns * TAU * 6., Effect_intensity);
}