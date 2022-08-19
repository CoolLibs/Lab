vec3 pixelation(
    vec3 in_color, float effect_intensity, vec2 uv,
    float nb_tiles_on_y_axis, float border, float smoothing, vec3 border_color
)
{
    vec2  gv           = fract(uv * nb_tiles_on_y_axis + 0.5) - 0.5;
    vec2  id           = floor(uv * nb_tiles_on_y_axis);
    float tiles_length = 1. / nb_tiles_on_y_axis;

    vec2 img_uv = (id + .5) * tiles_length;
    img_uv.x /= _aspect_ratio;

    float distance_to_center = smooth_max_polynomial((-abs(gv.x) + 1. - border), (-abs(gv.y) + 1. - border), smoothing);

    float epsilon = 0.01;

    vec3 out_color = mix(border_color, image(img_uv), 1. - smoothstep(border - epsilon, border + epsilon, distance_to_center));

    return mix(in_color, out_color, effect_intensity);
}