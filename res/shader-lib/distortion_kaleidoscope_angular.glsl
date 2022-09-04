// https://www.shadertoy.com/view/WdK3zt

vec2 distortion_kaleidoscope_angular(
    vec2 in_uv, float Effect_intensity,
    vec2 Center, float Angle_distortion, float strength, float strength2
)
{
    strength    = 1. / strength;
    vec2 adjVec = strength2 * vec2(
                                  cos(Angle_distortion),
                                  sin(Angle_distortion)
                              );
    vec2 uvM = mod(
        in_uv + normalize(in_uv - Center) * adjVec,
        strength
    );

    return mix(in_uv, uvM, Effect_intensity);
}