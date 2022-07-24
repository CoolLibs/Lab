#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/chromatic_aberration.glsl"

INPUT int Sample_count; // 20

INPUT float Falloff; // 1.082
INPUT float Blur;    // 0.142

INPUT float Center_x; // 0.142
INPUT float Center_y; // 0.142

INPUT float Effect_intensity;

void main()
{
    RgbColor in_color = image(_uv);

    RgbColor out_color = chromatic_aberration(
        in_color, Effect_intensity,
        Center_x, Center_y, Sample_count, Falloff, Blur
    );

    out_Color = vec4(out_color, 1.);
}