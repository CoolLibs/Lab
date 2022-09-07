#version 410

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/distortion_portholes.glsl"

INPUT bool  Allow_very_distorted_cells; /// This will also improve the movement over Time
INPUT float Scale;                      // 10
INPUT float Grid_irregularity;
INPUT float Shape;      // 2 /// Choses which norm is used to define the distance. 2 is the usual euclidian distance.
INPUT float Distortion; // 0.001
INPUT float Time;

INPUT float Effect_intensity;

void main()
{
    vec2 uv = distortion_portholes(
        normalized_uv(), Effect_intensity,
        !Allow_very_distorted_cells, Scale, Grid_irregularity, Time, Shape, Distortion
    );

    out_Color = vec4(image(uv), 1.);
}