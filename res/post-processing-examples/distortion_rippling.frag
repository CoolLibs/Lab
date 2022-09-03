#version 410

// #include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/distortion_rippling.glsl"

INPUT float Radius;
INPUT float Thickness; // default .1
// default .5 min = 0 max = 1
INPUT vec2 Center;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = _uv;

    vec2 out_uv = distortion_rippling(
        in_uv, Effect_intensity,
        Center, Radius, Thickness
    );

    vec3 color = image(out_uv);

    out_Color = vec4(color, 1.);
}