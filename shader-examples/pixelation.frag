#version 430

//#include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform float nb_tiles_on_y_axis; // default 20
uniform float border;             // default 0.469
uniform float smoothe;            // default 0.225

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

    vec2  gv           = fract(uv * nb_tiles_on_y_axis + 0.5) - 0.5;
    vec2  id           = floor(uv * nb_tiles_on_y_axis);
    float tiles_length = 1. / nb_tiles_on_y_axis;

    vec2 img_uv = (id + .5) * tiles_length;
    img_uv.x /= _aspect_ratio;

    float distance_to_center = smooth_max_polynomial((-abs(gv.x) + 1. - border),
                                                     (-abs(gv.y) + 1. - border),
                                                     smoothe);

    float epsilon = 0.01;
    color         = mix(border_color,
                        image(img_uv),
                        1. - smoothstep(border - epsilon, border + epsilon, distance_to_center));

    out_Color = vec4(color, 1.);
}