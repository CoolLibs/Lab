vec2 distortion_by_texture(
    vec2 in_uv, float mask, sampler2D texture_2D,
    float strength
)
{
    vec2 out_uv =
        in_uv + (texture(texture_2D, in_uv).r * 0.05 - 0.025) * strength * mask;
    return out_uv;
}