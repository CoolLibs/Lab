#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/color_effects.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/threshold_by_chosen_color.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/grid_9_colors.glsl"

// Inputs for threshold_by_chosen_color
INPUT int      Number_of_colors; // min 2
INPUT RgbColor Color0;

// Inputs for color_effects
// INPUT float Grey_coefficient;
// INPUT bool  One_color_mode;

// Grid
INPUT RgbColor Color1;
INPUT RgbColor Color2;
INPUT RgbColor Color3;
INPUT RgbColor Color4;
INPUT RgbColor Color5;
INPUT RgbColor Color6;
INPUT RgbColor Color7;
INPUT RgbColor Color8;
INPUT RgbColor Color9;

// TODO(ASG) Move in a shader-demo folder

void main()
{
    vec2 in_uv = _uv;

    vec2 out_uv = grid_9_colors(in_uv, Effect_intensity);

    vec3 in_color = image(fract(out_uv));

    const int size = 3;

    vec3 table_of_colors[size * size] = vec3[](
        Color1,
        Color2,
        Color3,
        Color4,
        Color5,
        Color6,
        Color7,
        Color8,
        Color9
    );

    ivec2 grid_id = ivec2(floor(out_uv));

    vec3 current_color = table_of_colors[size * grid_id.x + grid_id.y];

    vec3 color = threshold_by_chosen_color(
        in_color, Effect_intensity,
        Number_of_colors, Color0, current_color
    );

    // vec3 color = color_effects(
    //     in_color, Effect_intensity,
    //     current_color, Grey_coefficient, One_color_mode
    // );

    out_Color = vec4(color, 1.);
}