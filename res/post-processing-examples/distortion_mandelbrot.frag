#version 410

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/distortion_mandelbrot.glsl"

INPUT float   Scale;
INPUT Point2D Center;
INPUT float   Iterations;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = normalized_uv();

    vec2 out_uv = distortion_mandelbrot(
        in_uv, Effect_intensity,
        Center, Scale, Iterations
    );

    vec3 color = image(out_uv);

    out_Color = vec4(color, 1.);
}
