#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/chromatic_aberration.glsl"

// BEGIN DYNAMIC PARAMS

uniform int Sample_count; // 20

uniform float Falloff; // 1.082
uniform float Blur;    // 0.142

uniform float Center_x; // 0.142
uniform float Center_y; // 0.142

uniform float Effect_intensity;

// END DYNAMIC PARAMS

void main()
{
    vec3 in_color = image(_uv);

    vec3 out_color = chromatic_aberration(
        in_color, Effect_intensity,
        Center_x, Center_y, Sample_count, Falloff, Blur
    );

    out_Color = vec4(out_color, 1.);
}