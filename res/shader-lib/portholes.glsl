struct VoronoiRes {
    vec2  cid;
    float minDist;
    float minDist2;
};

vec2 distribute_center_in_a_circle(vec2 n, float t, vec2 offs, float Space_to_border)
{
    float radius   = Space_to_border * 0.5 * sqrt(n.x);
    float theta    = n.x * TAU * sin(t);
    vec2  p_circle = radius * vec2(cos(theta), sin(theta));
    vec2  p        = offs + p_circle;
    return p;
}

vec2 distribute_center_in_a_square(vec2 n, float t, vec2 offs, float Space_to_border, float size, float Speed, float Movement)
{
    vec2 p = offs + sin(n * t) * Space_to_border * .5;
    return p;
}

VoronoiRes voronoi(vec2 uv, bool Time_mode, float space_to_border, float size, float Speed, float Movement, bool Square_mode, float Distance_mode)
{
    float t = 0;
    if (Time_mode)
    {
        t = _time * Speed;
    }
    else
    {
        t = Movement;
    }

    VoronoiRes res;
    res.cid      = vec2(0);
    res.minDist  = 100.;
    res.minDist2 = 100.;
    uv *= size;
    vec2 gv  = fract(uv) - .5;
    vec2 id  = floor(uv);
    vec2 cid = vec2(0);

    for (float y = -1.; y <= 1.; y++)
    {
        for (float x = -1.; x <= 1.; x++)
        {
            vec2 offs = vec2(x, y);

            vec2 n = hash_0_to_1_2D_to_2D(id + offs);
            vec2 p;
            if (Square_mode)
            {
                p = distribute_center_in_a_square(n, t, offs, space_to_border, size, Speed, Movement);
            }
            else
            {
                p = distribute_center_in_a_circle(n, t, offs, space_to_border);
            }

            vec2  p_to_gv = p - gv;
            float d       = pow(
                      pow(abs(p_to_gv.x), Distance_mode) + pow(abs(p_to_gv.y), Distance_mode),
                      1 / Distance_mode
                  );

            if (d < res.minDist)
            {
                res.minDist2 = res.minDist;
                res.minDist  = d;
                res.cid      = id + offs;
            }
            else if (d < res.minDist2)
            {
                res.minDist2 = d;
            }
        }
    }
    return res;
}

vec2 portholes_uv(
    vec2 in_uv, float effect_intensity,
    float zoom_intensity,
    float zoom, vec2 grad
)
{
    return mix(in_uv, in_uv + grad * zoom * zoom_intensity, effect_intensity);
}

vec3 portholes(
    vec2 in_uv, float effect_intensity,
    bool square_mode, float size, float space_to_border, float speed, float movement, bool time_mode, float distance_mode, float zoom_intensity,
    float change_center, float border, float smoothing, float border_smoothing, vec3 border_color
)
{
    float minDist  = 100.;
    float minDist2 = 100.;

    VoronoiRes res  = voronoi(in_uv, time_mode, space_to_border, size, speed, movement, square_mode, distance_mode);
    VoronoiRes resX = voronoi(in_uv + vec2(0.001, 0.), time_mode, space_to_border, size, speed, movement, square_mode, distance_mode);
    VoronoiRes resY = voronoi(in_uv + vec2(0., 0.001), time_mode, space_to_border, size, speed, movement, square_mode, distance_mode);
    vec2       grad = vec2(resX.minDist - res.minDist, resY.minDist - res.minDist) /
                0.001;
    float distance_to_center = res.minDist / res.minDist2;
    float distance_to_edges  = 1. - distance_to_center;
    vec2  uv                 = portholes_uv(in_uv, effect_intensity, zoom_intensity, distance_to_edges, grad);

    vec3 out_color = mix(
        image(uv),
        border_color,
        smoothstep(
            border,
            1. - border,
            1 - smooth_max_polynomial(distance_to_edges, distance_to_edges * border_smoothing, smoothing)
        ) * effect_intensity
    );
    return out_color;
}