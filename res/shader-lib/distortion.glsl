vec2 distortion(vec2 in_uv, float effect_intensity) // add _texture
{
    vec2 out_uv = in_uv + (texture(_texture, _uv).r * 0.05 - 0.025) * effect_intensity;
    return out_uv;
}