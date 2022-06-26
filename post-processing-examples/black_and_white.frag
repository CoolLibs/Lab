#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/black_and_white.glsl"

// BEGIN DYNAMIC PARAMS

uniform float Red_contribution;   // default 0.2126
uniform float Green_contribution; // default 0.7152
uniform float Blue_contribution;  // default 0.0722
uniform float Normalize_checkbox;

uniform float Effect_intensity;

// END DYNAMIC PARAMS

void main()
{
    vec3 in_color = image(_uv);

    vec3 channels_contribution = vec3(Red_contribution, Green_contribution, Blue_contribution);

    vec3 out_color = black_and_white(
        in_color, Effect_intensity,
        channels_contribution, Normalize_checkbox
    );

    out_Color = vec4(out_color, 1.);
}
