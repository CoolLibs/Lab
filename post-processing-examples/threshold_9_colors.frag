#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/color_effects.glsl"
// #include "_ROOT_FOLDER_/shader-lib/grid_9_colors.glsl"

// uniform int number_of_colors; // min 2

// uniform RgbColor Color0;

input RgbColor Color_coefficient;

input float Grey_coefficient;
input bool  One_color_mode;

input RgbColor Color1;
input RgbColor Color2;
input RgbColor Color3;
input RgbColor Color4;
input RgbColor Color5;
input RgbColor Color6;
input RgbColor Color7;
input RgbColor Color8;
input RgbColor Color9;

input float Effect_intensity;

void main()
{
    vec2 in_uv = _uv;

    vec2 out_uv = grid_9_colors(in_uv, Effect_intensity);

    RgbColor in_color = image(fract(out_uv));

    const int size = 3;

    RgbColor table_of_colors[size][size] = {
        {Color1, Color2, Color3},
        {Color4, Color5, Color6},
        {Color7, Color8, Color9}};

    ivec2 grid_id = ivec2(floor(out_uv));

    RgbColor current_color = table_of_colors[grid_id.x][grid_id.y];

    // RgbColor out_color = threshold_by_chosen_color(
    //     in_color, Effect_intensity,
    //     number_of_colors, Color0, current_color
    // );

    RgbColor out_color = color_effects(
        in_color, Effect_intensity,
        current_color, Grey_coefficient, One_color_mode
    );

    out_Color = vec4(out_color, 1.);
}