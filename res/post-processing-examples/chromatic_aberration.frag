#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/chromatic_aberration.glsl"

INPUT int     Sample_count;
INPUT float   Falloff;
INPUT float   Blur;
INPUT Point2D Center;

INPUT float Effect_intensity;

void main()
{
    vec3 in_color = image(_uv);
    vec2 uv       = _uv - 0.5;
    uv.x *= _aspect_ratio;
    uv -= Center;

    vec3 color = chromatic_aberration(
        in_color, Effect_intensity, uv,
        Sample_count, Falloff, Blur
    );

    out_Color = vec4(color, 1.);
}