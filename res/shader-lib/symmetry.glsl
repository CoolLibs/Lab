// #include "_COOL_RES_/shaders/math.glsl"

float symmetry_side(bool flip, float ortho)
{
    return flip ? min(0, ortho) : max(0., ortho);
}

vec2 symmetry(
    vec2 in_uv, float mask,
    vec2 center, float angle_in_radians, bool flip
)
{
    vec2 u_line = vec2(sin(angle_in_radians), -cos(angle_in_radians));

    vec2 out_uv = in_uv - center;
    out_uv      = center + out_uv - u_line * symmetry_side(flip, dot(out_uv, u_line)) * 2.;

    return mix(in_uv, out_uv, mask);
}
