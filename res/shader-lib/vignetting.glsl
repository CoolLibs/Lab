// https://www.shadertoy.com/view/lsKSWR

vec3 vignetting(
    vec3 in_color, float effect_intensity, vec2 uv,
    float border_darkness, float center_radius, vec3 color
)
{
    float vignette = uv.x * uv.y * center_radius;
    vignette       = min(max(pow(vignette, border_darkness * effect_intensity), 0.), 1.);

    vec3 out_color = mix(color, in_color, vignette);

    return out_color;
}