#version 410

// #include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/distortion_black_hole.glsl"

INPUT Angle Angle_in_turns;    // 0 to 1 (1 == 1 turn)
INPUT float Hole_attractivity; // -1 to 1
// range 0 to 1
INPUT Point2D Hole_center; // vec2(0.5,0.5)
INPUT int     Subdivision; // 0 forbidden

INPUT float Effect_intensity;

/// Use Mirror_Reppeat instead of Clamp
void main()
{
    vec2 in_uv = _uv;

    vec2 out_uv = distortion_black_hole(
        in_uv, Effect_intensity,
        Hole_center, Angle_in_turns, Subdivision,
        Hole_attractivity
    );

    vec3 color = image(out_uv);

    out_Color = vec4(color, 1.);
}