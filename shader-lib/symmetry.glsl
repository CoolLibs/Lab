// #include "_COOL_RES_/shaders/math.glsl"

float symmetry_side(bool right, float ortho)
{
    return right ? min(0, ortho) : max(0., ortho);
}

vec2 symmetry(
    vec2 in_uv, float effect_intensity,
    float center_x, float center_y, float angle_in_turns, float right_or_left
)
{
    vec2  center = vec2(center_x, center_y);
    float angle  = angle_in_turns * TAU;
    vec2  u_line = vec2(sin(angle), -cos(angle));

    vec2 out_uv = in_uv - center;
    out_uv      = center + out_uv - u_line * symmetry_side(right_or_left > 0.5, dot(out_uv, u_line)) * 2.;

    return mix(in_uv, out_uv, effect_intensity);
}
