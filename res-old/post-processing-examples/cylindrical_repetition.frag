#version 410

uniform float _time;
out vec4      out_Color;

// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/cylindrical_repetition.glsl"

INPUT Angle   Angle;
INPUT float   Attractivity;
INPUT Point2D Center;
INPUT float   Subdivisions; /// Use a whole number if you don't want to see a seem, or check "Only whole subdivisions".
INPUT bool    Only_whole_subdivisions;

INPUT float Mask;

/// Use Mirror_Reppeat instead of Clamp
void main()
{
    vec2 in_uv = normalized_uv();

    vec2 out_uv = cylindrical_repetition(
        in_uv, Mask, _aspect_ratio,
        Center, Angle, Only_whole_subdivisions ? floor(Subdivisions) : Subdivisions, Attractivity
    );

    vec3 color = image(out_uv);

    out_Color = vec4(color, 1.);
}