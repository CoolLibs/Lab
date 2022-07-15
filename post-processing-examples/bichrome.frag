#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/define_types.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/bichrome.glsl"

// default 0.5 min -1 max 1

input RgbColor Color1;
input RgbColor Color2;
input float    Effect_intensity; // default 0.5 min -1 max 2

void main()
{
    RgbColor in_color = image(_uv);

    RgbColor out_color = bichrome(
        in_color, Effect_intensity,
        Color1, Color2
    );

    out_Color = vec4(out_color, 1.);
}