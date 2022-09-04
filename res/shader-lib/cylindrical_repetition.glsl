// https://www.shadertoy.com/view/MtKGzt

// #include "_COOL_RES_/shaders/math.glsl"

vec2 cylindrical_repetition(
    vec2 in_uv, float effect_intensity,
    vec2 Center, float rotation_in_radians, float Subdivision,
    float Attractivity
)
{
    vec2 coord = (in_uv - Center);

    // sink Hole projection
    float angle  = atan(coord.y, coord.x) - rotation_in_radians;
    float radius = length(coord);

    angle *= Subdivision / TAU * _aspect_ratio;
    radius += Attractivity;

    vec2 uv = vec2(angle, radius);
    return mix(in_uv, uv, effect_intensity);
}