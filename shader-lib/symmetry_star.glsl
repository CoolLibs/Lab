// https://www.youtube.com/watch?v=il_Qg9AqQkE

// #include "_COOL_RES_/shaders/math.glsl"

float symmetry_side(float right, float ortho)
{
    return (right > 0.5) ? min(0, ortho) : max(0., ortho);
}

vec2 u(float angle)
{
    return vec2(sin(angle), cos(angle));
}
vec2 symmetry_star(
    vec2 in_uv, float effect_intensity,
    float size, float angle_in_turns, float right_or_left, float nb_iterations
)
{
    vec2 out_uv = in_uv / size;

    out_uv.x            = abs(out_uv.x);
    float fractal_angle = (5. / 6.) * PI;
    out_uv.y += tan(fractal_angle) * .5;

    vec2  u_line = u(fractal_angle);
    float d      = dot(out_uv - vec2(.5, 0), u_line);
    out_uv -= u_line * symmetry_side(right_or_left, d) * 2;

    u_line = u(angle_in_turns * (2. / 3.) * PI);
    out_uv.x += .5;

    float scale = 3.;
    for (int i = 0; i < nb_iterations; i++)
    {
        out_uv *= scale;
        out_uv.x -= scale / 2.;
        out_uv.x = abs(out_uv.x);
        out_uv.x -= .5;
        out_uv -= u_line * min(0., dot(out_uv, u_line)) * 2;
    }

    d = length(out_uv - vec2(clamp(out_uv.x, -1, 1), 0.));
    out_uv /= (pow(scale, float(nb_iterations)));
    out_uv *= size;

    return mix(in_uv, out_uv, effect_intensity);
}