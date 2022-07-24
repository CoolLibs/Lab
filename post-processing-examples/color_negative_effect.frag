#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"

INPUT vec3 channels_contribution; // default 1 min = 0

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    vec2     uv    = _uv;
    RgbColor color = RgbColor(1.);
    vec4     img   = image(uv);
    color -= channels_contribution * img.rgb;
    out_Color = vec4(color, 1.);
}