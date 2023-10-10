// https://www.shadertoy.com/view/lsKSWR

vec3 vignetting(
    vec3 in_color, float mask, vec2 in_uv_unnormalized, float img_aspect_ratio,
    float strength, float radius, vec3 color
)
{
    vec2 vignette_uv = in_uv_unnormalized;
    vignette_uv *= 1.0 - vignette_uv.yx;
    vignette_uv.x *= img_aspect_ratio;
    float vignette = vignette_uv.x * vignette_uv.y * radius;
    vignette       = min(max(pow(vignette, strength * mask), 0.), 1.);

    vec3 out_color = mix(color, in_color, vignette);

    return out_color;
}