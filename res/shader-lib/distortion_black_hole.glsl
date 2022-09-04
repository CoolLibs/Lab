// https://www.shadertoy.com/view/MtKGzt

// TODO(ASG) Rename the effect

vec2 distortion_black_hole(
    vec2 in_uv, float effect_intensity,
    vec2 Center, float Angle_in_turns, float Subdivision,
    float Attractivity

)
{
    vec2 coord  = (in_uv - Center) * 2.;
    vec2 ncoord = coord;

    // rotation
    float angleRot = Angle_in_turns * TAU;
    vec2  tcoord   = vec2(ncoord.x * cos(angleRot) - ncoord.y * sin(angleRot), ncoord.y * cos(angleRot) + ncoord.x * sin(angleRot));

    // sink Hole projection
    float angle  = atan(tcoord.x, tcoord.y);
    float radius = length(tcoord);
    vec2  uv     = vec2(angle, radius);

    uv /= radians(360. / float(Subdivision));
    uv.y += Attractivity;

    return mix(in_uv, uv, effect_intensity);
}