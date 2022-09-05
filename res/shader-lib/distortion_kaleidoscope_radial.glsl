// https://www.shadertoy.com/view/WdK3zt

vec2 distortion_kaleidoscope_radial(
    vec2 in_uv, float effect_intensity,
    vec2 center, float distortion_angle, float strength, float strength2
)
{
    strength    = 1. / strength;
    vec2 adjVec = strength2 * vec2(
                                  cos(distortion_angle),
                                  sin(distortion_angle)
                              );
    vec2 uvM = in_uv;

    vec2 uv = normalize(uvM + center) * adjVec;

    if (mod(floor(uv.x / strength), 2.) == 0.)
    {
        uvM.x *= -1.;
    }
    if (mod(floor(uv.y / strength), 2.) == 0.)
    {
        uvM.y *= -1.;
    }
    uvM += adjVec * length(uvM + adjVec);

    return mix(in_uv, uvM, effect_intensity);
}