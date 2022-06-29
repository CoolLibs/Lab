// #include "_COOL_RES_/shaders/math.glsl"

// https://youtu.be/l-07BXzNdPw

RgbColor voronoi(
    RgbColor in_color, float effect_intensity, vec2 in_uv,
    float scale, float speed, float movement, bool time_mode, float distance_mode
)
{
    float m = 0.;
    float t = 0;
    if (time_mode)
    {
        t = _time * speed;
    }
    else
    {
        t = movement;
    }

    float minimal_distance = 100.;

    in_uv *= scale;
    vec2 gv  = fract(in_uv) - .5;
    vec2 id  = floor(in_uv);
    vec2 cid = vec2(0);

    for (float y = -1.; y <= 1.; y++)
    {
        for (float x = -1.; x <= 1.; x++)
        {
            vec2 offs = vec2(x, y);

            vec2 n = hash_0_to_1_2D_to_2D(id + offs);
            vec2 p = offs + sin(n * t) * .5;
            p -= gv;
            float d = pow(
                pow(abs(p.x), distance_mode) + pow(abs(p.y), distance_mode),
                1 / distance_mode
            );

            if (d < minimal_distance)
            {
                minimal_distance = d;
                cid              = id + offs;
            }
        }
    }
    RgbColor out_color = RgbColor(minimal_distance);

    return mix(in_color, out_color, effect_intensity);
}