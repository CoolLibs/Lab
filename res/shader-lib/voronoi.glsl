// #include "_COOL_RES_/shaders/math.glsl"

// https://youtu.be/l-07BXzNdPw

float voronoi(
    vec2  in_uv,
    float scale, float time, float shape
)
{
    float m = 0.;

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
            vec2 p = offs + sin(n * time) * .5;
            p -= gv;
            float d = pow(
                pow(abs(p.x), shape) + pow(abs(p.y), shape),
                1 / shape
            );

            if (d < minimal_distance)
            {
                minimal_distance = d;
                cid              = id + offs;
            }
        }
    }
    return minimal_distance;
}
vec2 voronoi_uv(
    vec2 in_uv, float effect_intensity,
    float scale, float time, float shape
)
{
    return mix(
        in_uv,
        vec2(voronoi(
            in_uv,
            scale, time, shape
        )),
        effect_intensity
    );
}

vec3 voronoi_color(
    vec2  in_uv,
    float scale, float time, float shape
)
{
    return vec3(voronoi(
        in_uv,
        scale, time, shape
    ));
}
