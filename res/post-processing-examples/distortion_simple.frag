#version 410

// #include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/distortion_simple.glsl"

// default 0.5 range 0 to 1
// Point2D
INPUT vec2 Center;

INPUT float Effect_intensity; // default 1 min 0 max 1

void main()
{
    vec2 in_uv = _uv;

    vec2 out_uv = distortion_simple(
        in_uv, Effect_intensity,
        Center
    );

    vec3 color = image(out_uv);

    out_Color = vec4(color, 1.);
}