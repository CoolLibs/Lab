// https://www.shadertoy.com/view/MtKGzt

// #include "_COOL_RES_/shaders/math.glsl"

vec2 cylindrical_repetition(
    vec2 in_uv, float mask, float aspect_ratio,
    vec2 center, float rotation_in_radians, float subdivisions, float attractivity
)
{
    vec2 coord = in_uv - center;

    // sink Hole projection
    float angle  = atan(coord.y, coord.x) - rotation_in_radians;
    float radius = length(coord);

    angle *= subdivisions / TAU * aspect_ratio;
    radius += attractivity;

    vec2 uv = vec2(angle, radius);
    return mix(in_uv, uv + center, mask);
}