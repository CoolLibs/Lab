// #include "_COOL_RES_/shaders/math.glsl"

struct VoronoiRes {
    vec2  cid;
    float minDist;
    float minDist2;
};

vec2 distribute_center_in_a_circle(vec2 n, float t, vec2 offs, float grid_irregularity)
{
    float radius   = grid_irregularity * 0.5 * sqrt(n.x);
    float theta    = n.x * TAU * sin(t);
    vec2  p_circle = radius * vec2(cos(theta), sin(theta));
    vec2  p        = offs + p_circle;
    return p;
}

vec2 distribute_center_in_a_square(vec2 n, float t, vec2 offs, float grid_irregularity, float scale)
{
    vec2 p = offs + sin(n * t) * grid_irregularity * .5;
    return p;
}

VoronoiRes voronoi(vec2 uv, float grid_irregularity, float scale, float time, bool keep_points_in_a_disk, float shape)
{
    VoronoiRes res;
    res.cid      = vec2(0);
    res.minDist  = FLT_MAX;
    res.minDist2 = FLT_MAX;
    uv *= scale;
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
            if (keep_points_in_a_disk)
            {
                p = distribute_center_in_a_circle(n, time, offs, grid_irregularity);
            }
            else
            {
                p = distribute_center_in_a_square(n, time, offs, grid_irregularity, scale);
            }

            vec2  p_to_gv = p - gv;
            float d       = pow(
                      pow(abs(p_to_gv.x), shape) + pow(abs(p_to_gv.y), shape),
                      1 / shape
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
    float distortion,
    float zoom, vec2 grad
)
{
    return mix(in_uv, in_uv + grad * zoom * distortion, effect_intensity);
}

vec2 distortion_portholes(
    vec2 in_uv, float effect_intensity,
    bool keep_points_in_a_disk, float scale, float grid_irregularity, float time, float shape, float distortion
)
{
    float minDist  = FLT_MAX;
    float minDist2 = FLT_MAX;

    VoronoiRes res  = voronoi(in_uv, grid_irregularity, scale, time, keep_points_in_a_disk, shape);
    VoronoiRes resX = voronoi(in_uv + vec2(0.001, 0.), grid_irregularity, scale, time, keep_points_in_a_disk, shape);
    VoronoiRes resY = voronoi(in_uv + vec2(0., 0.001), grid_irregularity, scale, time, keep_points_in_a_disk, shape);
    vec2       grad = vec2(resX.minDist - res.minDist, resY.minDist - res.minDist) /
                0.001;
    float distance_to_center = res.minDist / res.minDist2;
    float distance_to_edges  = 1. - distance_to_center;
    vec2  uv                 = portholes_uv(in_uv, effect_intensity, distortion, distance_to_edges, grad);

    return mix(in_uv, uv, effect_intensity);
}