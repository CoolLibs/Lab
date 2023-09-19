#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/flare.glsl"

INPUT Direction2D Direction;
INPUT Point2D     Center;
INPUT float       Scale;
INPUT float       Brightness;
INPUT int         Number_of_circles;
INPUT int         Seed;

INPUT float Mask;

void main()
{
    vec2 in_uv    = normalized_uv();
    vec3 in_color = image(in_uv);

    vec3 color = flare(
        in_color, Mask, in_uv,
        Center, Direction, Scale, Brightness, Number_of_circles, Seed
    );

    out_Color = vec4(color, 1.);
}