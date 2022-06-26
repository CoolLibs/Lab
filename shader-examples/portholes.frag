#version 430

//#include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform float Square_mode; // bool
uniform float Size;        // 10
uniform float Space_to_border;
uniform float Speed;
uniform float Movement;
uniform float Time_mode;      // bool
uniform float Distance_mode;  // 2
uniform float Zoom_intensity; // 0.001
uniform float Translation_On_Off;

uniform float change_center;
uniform float border;
uniform float smoothe;
uniform float border_smooth;

uniform vec3 border_color;

// END DYNAMIC PARAMS

vec3 image(vec2 uv)
{
    return texture2D(_image, uv).rgb;
}

struct VoronoiRes {
    vec2  cid;
    float minDist;
    float minDist2;
};

vec2 distribute_center_in_a_circle(vec2 n, float t, vec2 offs)
{
    float radius   = Space_to_border * 0.5 * sqrt(n.x);
    float theta    = n.x * TAU * sin(t);
    vec2  p_circle = radius * vec2(cos(theta),
                                   sin(theta));
    vec2  p        = offs + p_circle;
    return p;
}

vec2 distribute_center_in_a_square(vec2 n, float t, vec2 offs)
{
    vec2 p = offs + sin(n * t) * Space_to_border * .5;
    return p;
}

VoronoiRes voronoi(vec2 uv)
{
    float t = 0;
    if (Time_mode > .5)
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
    uv *= Size;
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
            if (Square_mode > .5)
            {
                p = distribute_center_in_a_square(n, t, offs);
            }
            else
            {
                p = distribute_center_in_a_circle(n, t, offs);
            }

            vec2  p_to_gv = p - gv;
            float d       = pow(
                      pow(abs(p_to_gv.x), Distance_mode) + pow(abs(p_to_gv.y), Distance_mode),
                      1 / Distance_mode);

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

void main()
{
    float m = 0.;

    float minDist   = 100.;
    float minDist2  = 100.;
    float cellIndex = 0.;

    vec2 uv    = _uv;
    vec3 color = image(uv);
    uv.x *= _aspect_ratio;

    vec2  gv           = fract(uv * Size + change_center) - change_center;
    vec2  id           = floor(uv * Size);
    float tiles_length = 1. / Size;

    vec2 img_uv = (id + .5) * tiles_length;
    img_uv.x /= _aspect_ratio;

    VoronoiRes res  = voronoi(uv);
    VoronoiRes resX = voronoi(uv + vec2(0.001, 0.));
    VoronoiRes resY = voronoi(uv + vec2(0., 0.001));
    vec2       grad = vec2(resX.minDist - res.minDist,
                           resY.minDist - res.minDist) /
                0.001;
    float distance_to_center = res.minDist / res.minDist2;
    float distance_to_edges  = 1. - distance_to_center;
    float zoom               = distance_to_edges;

    vec2 uv2 = _uv;

    color = mix(image(uv2 + grad * zoom * Zoom_intensity), border_color,
                smoothstep(
                    border,
                    1. - border,
                    1 - smooth_max_polynomial(distance_to_edges,
                                              distance_to_edges * border_smooth,
                                              smoothe)));

    out_Color = vec4(color, 1.);
    // out_Color = vec4(vec3(res.minDist), 1.);
}