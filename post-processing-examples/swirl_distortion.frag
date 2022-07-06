#version 430

// https://www.shadertoy.com/view/Xscyzn

// #include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/define_types.glsl"

// BEGIN DYNAMIC PARAMS

uniform float center_x; // default 0.5
uniform float center_y; // default 0.5

uniform float size;    // only positive values
uniform float nb_turn; // positive and negative values

// END DYNAMIC PARAMS

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    vec2 center = vec2(center_x, center_y);
    center      = center == vec2(0., 0.) ? vec2(.5, .5) : center;

    vec2 uv = _uv - center;

    float len    = length(uv * vec2(_aspect_ratio, 1.));
    float angle  = atan(uv.y, uv.x) + nb_turn * TAU * smoothstep(size, 0., len);
    float radius = length(uv);

    RgbColor color = image(vec2(radius * cos(angle), radius * sin(angle)) + center).rgb;

    out_Color = vec4(color, 1.);
}