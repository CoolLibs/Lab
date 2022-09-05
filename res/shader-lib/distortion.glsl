vec2 distortion(vec2 in_uv, float effect_intensity, sampler2D texture_2D)
{
    vec2 out_uv =
        in_uv +
        (texture(texture_2D, in_uv).r * 0.05 - 0.025
        ) *
            effect_intensity;
    return out_uv;
}