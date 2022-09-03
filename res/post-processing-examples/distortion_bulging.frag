#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/distortion_bulging.glsl"

// default 0.5 range 0 to 1
INPUT Point2D Center;
INPUT float   Strength;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = _uv;

    vec2 out_uv = distortion_bulging(
        in_uv, Effect_intensity,
        Strength, Center
    );
    vec3 out_color = image(out_uv);

    out_Color = vec4(out_color, 1.);
}