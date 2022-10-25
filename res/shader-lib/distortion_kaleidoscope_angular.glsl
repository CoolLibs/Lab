// https://www.shadertoy.com/view/WdK3zt

vec2 distortion_kaleidoscope_angular(
    vec2 in_uv, float effect_intensity,
    vec2 center, float distortion_angle, float strength, float strength2
)
{
    strength    = 1. / strength;
    vec2 adjVec = strength2
                  * vec2(
                      cos(distortion_angle),
                      sin(distortion_angle)
                  );
    vec2 uvM = mod(
        in_uv + normalize(in_uv - center) * adjVec,
        strength
    );

    return mix(in_uv, uvM, effect_intensity);
}