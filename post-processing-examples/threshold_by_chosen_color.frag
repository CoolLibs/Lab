#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/define_types.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/threshold_by_chosen_color.glsl"

// BEGIN DYNAMIC PARAMS

uniform int number_of_colors;

uniform RgbColor Color1;
uniform RgbColor Color2;

uniform float Effect_intensity;

// END DYNAMIC PARAMS

void main()
{
    RgbColor in_color = image(_uv);

    RgbColor out_color = threshold_by_chosen_color(
        in_color, Effect_intensity,
        number_of_colors, Color1, Color2
    );

    out_Color = vec4(out_color, 1.);
}