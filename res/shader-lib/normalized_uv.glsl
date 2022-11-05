layout(location = 0) in vec2 _uv;
uniform float _aspect_ratio;

vec2 normalize_uv(vec2 uv)
{
    uv -= 0.5;
    uv.x *= _aspect_ratio;
    return uv * 2.;
}

vec2 normalized_uv()
{
    return normalize_uv(_uv);
}

vec2 unnormalize_uv(vec2 uv)
{
    uv /= 2.;
    uv.x /= _aspect_ratio;
    uv += 0.5;
    return uv;
}