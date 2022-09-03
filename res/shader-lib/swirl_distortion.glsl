// https://www.shadertoy.com/view/Xscyzn

vec2 swirl_distortion(
    vec2 in_uv, float Effect_intensity,
    vec2 Center, float Size
)
{
    vec2 center = Center == vec2(0., 0.) ? vec2(.5, .5) : Center;
    vec2 uv     = in_uv - center;

    float len    = length(uv * vec2(_aspect_ratio, 1.));
    float angle  = atan(uv.y, uv.x) + Effect_intensity * TAU * smoothstep(Size, 0., len);
    float radius = length(uv);

    return vec2(radius * cos(angle), radius * sin(angle)) + center;
}