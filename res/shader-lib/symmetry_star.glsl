// https://www.youtube.com/watch?v=il_Qg9AqQkE

// #include "_COOL_RES_/shaders/math.glsl"

vec2 u(float angle)
{
    return vec2(sin(angle), cos(angle));
}

vec2 symmetry_star(
    vec2 in_uv, float effect_intensity,
    float aspect_ratio,
    vec2 Center, float size, float angle_in_turns, float nb_iterations
)
{
    vec2 out_uv = in_uv - Center;
    out_uv.x *= aspect_ratio;
    out_uv /= size;

    out_uv.x            = abs(out_uv.x);
    float fractal_angle = (5. / 6.) * PI;
    out_uv.y += tan(fractal_angle) * 0.5;

    vec2  u_line = u(fractal_angle);
    float d      = dot(out_uv - vec2(0.5, 0), u_line);
    out_uv -= u_line * max(0., d) * 2.;

    u_line = u(angle_in_turns * (2. / 3.) * PI);
    out_uv.x += 0.5;

    float scale = 3.;
    for (int i = 0; i < nb_iterations; i++)
    {
        out_uv *= scale;
        out_uv.x -= scale / 2.;
        out_uv.x = abs(out_uv.x);
        out_uv.x -= 0.5;
        out_uv -= u_line * min(0., dot(out_uv, u_line)) * 2.;
    }

    d = length(out_uv - vec2(clamp(out_uv.x, -1., 1.), 0.));
    out_uv /= (pow(scale, float(nb_iterations)));
    out_uv *= size;
    out_uv += Center;

    return mix(in_uv, out_uv, effect_intensity);
}