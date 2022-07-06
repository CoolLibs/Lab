vec3 gradient_overlay(
    vec3 in_color, float effect_intensity,
    vec2 gradient_pos_a, vec2 gradient_pos_b, vec3 gradient_color_a, vec3 gradient_color_b, float gradient_presence, float use_the_best_blending_technique
)
{
    vec2 uv = _uv;
    uv.x *= _aspect_ratio;
    // gradient overlay
    vec2  a_to_p         = uv - gradient_pos_a;
    vec2  a_to_b         = gradient_pos_b - gradient_pos_a;
    float atb2           = dot(a_to_b, a_to_b);
    float atp_dot_atb    = dot(a_to_p, a_to_b);
    float t              = clamp(atp_dot_atb / atb2, 0.0, 1.0);
    vec3  gradient_color = mix(gradient_color_a, gradient_color_b, t);
    vec3  color          = use_the_best_blending_technique >= 0.5
                               ? mix(in_color, in_color * gradient_color, gradient_presence)
                               : mix(in_color, gradient_color, gradient_presence);

    vec3 out_color = color;
    return mix(in_color, out_color, effect_intensity);
}