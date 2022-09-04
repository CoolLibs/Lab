#version 410

// #include "_COOL_RES_/shaders/math.glsl"

uniform float _time;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/distortion_log_polar.glsl"

// default 0.5 range 0 to 1 //Point2D
INPUT vec2  Center;
INPUT float Scale1; //  range 0 to 7
INPUT float Scale2;
INPUT Angle Angle_in_turns; // 0 to 1 (1 == 1 turn)
INPUT float Flowerness;     // default 0.01
INPUT int   Subdivision;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = normalized_uv();

    vec2 out_uv = distortion_log_polar(
        in_uv, Effect_intensity,
        Center, Scale1, Scale2, Angle_in_turns, Flowerness, Subdivision
    );

    vec3 color = image(out_uv);

    out_Color = vec4(color, 1.0);
}