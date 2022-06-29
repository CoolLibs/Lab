// https://www.shadertoy.com/view/lsKSWR

RgbColor vignetting(
    RgbColor in_color, float effect_intensity, vec2 uv,
    float border_darkness, float center_radius
)
{
    float vignette = uv.x * uv.y * center_radius;
    vignette       = min(max(pow(vignette, border_darkness), 0.), 1.);

    RgbColor out_color = in_color * vignette;

    return mix(in_color, out_color, effect_intensity);
}