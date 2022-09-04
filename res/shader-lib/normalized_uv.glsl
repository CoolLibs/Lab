layout(location = 0) in vec2 _uv;
uniform float _aspect_ratio;

vec2 normalized_uv()
{
    vec2 uv = _uv - 0.5;
    uv.x *= _aspect_ratio;
    return uv;
}