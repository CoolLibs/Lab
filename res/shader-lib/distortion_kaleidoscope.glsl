// https://www.shadertoy.com/view/WdK3zt

vec2 distortion_kaleidoscope(
    vec2 in_uv, float Effect_intensity,
    vec2 Center, float Angle_distortion, float strength
)
{
    strength    = 1 / strength;
    vec2 adjVec = vec2((sin(Angle_distortion) * .5), (cos(Angle_distortion) * .5));
    vec2 uvM    = mod(in_uv + normalize(in_uv - Center) * adjVec, strength);
    vec2 uv     = normalize(uvM - vec2(strength / 2.)) * (adjVec + vec2(.5));

    if (mod(floor((uv.x) / strength), 2.) == 0.)
    {
        uvM.x = strength - uvM.x;
    }
    if (mod(floor((uv.y) / strength), 2.) == 0.)
    {
        uvM.y = strength - uvM.y;
    }
    uvM += adjVec * length(uvM + adjVec - vec2(0.5));

    return mix(in_uv, uvM, Effect_intensity);
}