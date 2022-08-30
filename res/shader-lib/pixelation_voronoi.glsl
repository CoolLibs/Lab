// #include "_COOL_RES_/shaders/math.glsl"

vec2 pixelation_voronoi(
    vec2 in_uv, float effect_intensity,
    float size, float speed, float movement, bool time_mode, float distance_mode
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

    float minDist   = 100.;
    float cellIndex = 0.;

    vec3 col = vec3(0);

    vec2 uv  = in_uv * size;
    vec2 gv  = fract(uv) - .5;
    vec2 id  = floor(uv);
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

            if (d < minDist)
            {
                minDist = d;
                cid     = id + offs;
            }
        }
    }
    vec2 image_uv = cid / size;

    return mix(in_uv, image_uv, effect_intensity);
}