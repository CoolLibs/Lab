#version 410

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/pixelation_voronoi.glsl"

INPUT float Scale;
INPUT float Shape; /// Chooses which norm is used to define the distance. 2 is the usual euclidian distance.
INPUT float Time;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = normalized_uv();

    vec2 out_uv = pixelation_voronoi(
        in_uv, Effect_intensity,
        Scale, Time, Shape
    );

    vec3 color = image(out_uv);

    out_Color = vec4(color, 1.);
}