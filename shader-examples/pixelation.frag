#version 430

//#include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform float nb_tiles_on_v;
uniform float change_center;
uniform float border;
uniform float smoothe;

uniform vec3 border_color;

// END DYNAMIC PARAMS

vec3 image(vec2 uv)
{
    return texture2D(_image, uv).rgb;
}

void main()
{
    vec2 uv    = _uv;
    vec3 color = image(uv);
    uv.x *= _aspect_ratio;

    vec2  gv           = fract(uv * nb_tiles_on_v + change_center) - change_center;
    vec2  id           = floor(uv * nb_tiles_on_v);
    float tiles_length = 1. / nb_tiles_on_v;

    vec2 img_uv = (id + .5) * tiles_length;
    img_uv.x /= _aspect_ratio;

    color = mix(image(img_uv), border_color,
                1. - smoothstep(
                         smooth_max_polynomial((-abs(gv.x) + 1. - border),
                                               (-abs(gv.y) + 1. - border),
                                               smoothe),
                         border,
                         1. - border));

    out_Color = vec4(color, 1.);
}