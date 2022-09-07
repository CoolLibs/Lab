#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/symmetry.glsl"

INPUT Point2D Center;
INPUT Angle   Angle;
INPUT bool    Flip;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = normalized_uv();

    vec2 out_uv = symmetry(
        in_uv, Effect_intensity,
        Center, Angle, Flip
    );

    vec3 color = image(out_uv);

    out_Color = vec4(color, 1.);
}
