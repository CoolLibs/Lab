// https://www.shadertoy.com/view/WdK3zt

vec2 distortion_kaleidoscope(
    vec2 in_uv, float Effect_intensity,
    vec2 Center, Angle Angle_distortion, float Scale
)
{
    vec2 adjVec = vec2((sin(Angle_distortion) * .5), (cos(Angle_distortion) * .5));
    vec2 uvM    = mod(in_uv + normalize(in_uv - Center) * adjVec, Scale);
    in_uv       = normalize(uvM - vec2(Scale / 2.)) * (adjVec + vec2(.5));

    if (mod(floor((in_uv.x) / Scale), 2.) == 0.)
    {
        uvM.x = Scale - uvM.x;
    }
    if (mod(floor((in_uv.y) / Scale), 2.) == 0.)
    {
        uvM.y = Scale - uvM.y;
    }
    uvM += adjVec * length(uvM + adjVec - vec2(0.5));

    return mix(in_uv, uvM, Effect_intensity);
}