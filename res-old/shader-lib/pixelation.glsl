vec2 pixelation(
    vec2 in_uv, float mask,
    float nb_tiles_on_y_axis
)
{
    vec2  id           = floor(in_uv * nb_tiles_on_y_axis);
    float tiles_length = 1. / nb_tiles_on_y_axis;

    return mix(in_uv, (id + .5) * tiles_length, mask);
}

vec3 pixelation_with_border(
    vec2 in_uv, float mask,
    float nb_tiles_on_y_axis, float border, float border_roundness, vec3 border_color
)
{
    vec2 img_uv = pixelation(in_uv, mask, nb_tiles_on_y_axis);

    vec2  gv                 = fract(in_uv * nb_tiles_on_y_axis + 0.5) - 0.5;
    float distance_to_center = smooth_max_polynomial((-abs(gv.x) + 1. - border), (-abs(gv.y) + 1. - border), border_roundness);
    float epsilon            = 0.01;
    vec3  out_color          = mix(
        border_color,
        image(img_uv),
        1. - smoothstep(border - epsilon, border + epsilon, distance_to_center) * mask
    );

    return out_color;
}