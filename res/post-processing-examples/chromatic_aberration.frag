#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/chromatic_aberration.glsl"

INPUT int Sample_count; // 20

INPUT float Falloff; // default 1.082 min 0. max 5.
INPUT float Blur;    // default 0.142 min -1. max 1.

// Point2D
INPUT vec2 Center; // default vec2(0.5,0.5)

INPUT float Effect_intensity; // default 1. min 0. max 1.

void main()
{
    vec3 in_color = image(_uv);

    vec3 out_color = chromatic_aberration(
        in_color, Effect_intensity,
        _uv, _aspect_ratio,
        Center, Sample_count, Falloff, Blur
    );

    out_Color = vec4(out_color, 1.);
}