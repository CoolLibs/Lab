#version 410

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/symmetry_star.glsl"

INPUT float   Scale; // 0 forbidden 0.001 to 0.5
INPUT Angle   angle;
INPUT int     Iterations;
INPUT Point2D Center;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = normalized_uv();

    vec2 out_uv = symmetry_star(
        in_uv, Effect_intensity,
        Center, Scale, angle, Iterations
    );

    vec3 color = image(out_uv);

    out_Color = vec4(color, 1.);
}