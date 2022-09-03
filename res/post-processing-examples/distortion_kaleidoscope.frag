#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/distortion_kaleidoscope.glsl"

INPUT Angle   Angle_distortion;
INPUT float   Scale;
INPUT Point2D Center; // default vec2(.5,.5)

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = _uv;
    in_uv -= 0.5;
    in_uv.x *= _aspect_ratio; // TODO(ASG check if we need to take aspect ratio into account in ALL shaders)

    vec2 out_uv = distortion_kaleidoscope(
        in_uv, Effect_intensity,
        Center, Angle_distortion, Scale
    );

    vec3 out_color = image(out_uv);

    out_Color = vec4(out_color, 1.);
}