#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/voronoi.glsl"

INPUT float Scale; // 10
INPUT float Time;
INPUT float Shape; // 2 /// Chooses which norm is used to define the distance. 2 is the usual euclidian distance.

INPUT float Mask;

void main()
{
    vec2 in_uv = normalized_uv();

    vec2 out_uv = voronoi_uv(
        in_uv, Mask,
        Scale, Time, Shape
    );

    vec3 color = image(out_uv);

    out_Color = vec4(color, 1.);
}