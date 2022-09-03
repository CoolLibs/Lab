// #include "_COOL_RES_/shaders/math.glsl"

float symmetry_side(bool right, float ortho)
{
    return right ? min(0, ortho) : max(0., ortho);
}

vec2 symmetry(
    vec2 in_uv, float effect_intensity,
    vec2 center, float angle_in_turns, bool right_or_left
)
{
    float angle  = angle_in_turns * TAU;
    vec2  u_line = vec2(sin(angle), -cos(angle));

    vec2 out_uv = in_uv - center;
    out_uv      = center + out_uv - u_line * symmetry_side(right_or_left, dot(out_uv, u_line)) * 2.;

    return mix(in_uv, out_uv, effect_intensity);
}
