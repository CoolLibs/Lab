#version 410

// https://www.shadertoy.com/view/stSGDW

// #include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"

INPUT float center_x; // default 0.5 range 0 to 1
INPUT float center_y; // default 0.5 range 0 to 1
INPUT float size;
INPUT float details;

INPUT int distor;

void main()
{
    vec2 uv     = _uv;
    vec2 center = vec2(center_x, center_y);
    uv -= center;
    uv.x *= _aspect_ratio;
    // uv *= size;

    float r = sqrt(uv.x * uv.x + uv.y * uv.y) * size / 15.0;

    float a = 60.0 * sin(distor * atan(uv.x + uv.y / r, uv.y - uv.x / r));
    a += 500.0;
    a = a * (r / 50.0);
    a = 200000.0 * sin(a * 6.0) * (r / 30.0) * details;

    vec3 col = image(uv + vec2(cos(a / 20.0), cos(a / 200.0)) - center).rgb;

    out_Color = vec4(col, 1.0);
}