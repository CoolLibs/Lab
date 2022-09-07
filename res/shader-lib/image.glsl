uniform sampler2D _image;

vec3 image(vec2 uv)
{
    uv.x /= _aspect_ratio;
    uv += 0.5;
    return texture2D(_image, uv).rgb;
}