vec3 gradient_overlay(
    vec3 in_color, float effect_intensity, vec2 in_uv,
    vec2 gradient_pos_a, vec2 gradient_pos_b, vec3 gradient_color_a, vec3 gradient_color_b, float gradient_presence
)
{
    // gradient overlay
    vec2  a_to_p         = in_uv - gradient_pos_a;
    vec2  a_to_b         = gradient_pos_b - gradient_pos_a;
    float atb2           = dot(a_to_b, a_to_b);
    float atp_dot_atb    = dot(a_to_p, a_to_b);
    float t              = clamp(atp_dot_atb / atb2, 0.0, 1.0);
    vec3  gradient_color = mix(gradient_color_a, gradient_color_b, t);
    vec3  color          = mix(
                  in_color,
                  in_color * gradient_color,
                  gradient_presence * effect_intensity
              );

    return color;
}