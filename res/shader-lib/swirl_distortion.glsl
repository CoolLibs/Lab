// https://www.shadertoy.com/view/Xscyzn

vec2 swirl_distortion(
    vec2 in_uv, float effect_intensity,
    vec2 center, float srength, float size
)
{
    vec2 uv = in_uv - center;

    float len    = length(uv);
    float angle  = atan(uv.y, uv.x) + srength * effect_intensity * TAU * smoothstep(size, 0., len);
    float radius = length(uv);

    return vec2(radius * cos(angle), radius * sin(angle)) + center;
}