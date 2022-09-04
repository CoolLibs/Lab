#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/vignetting.glsl"

INPUT float Border_darkness; // Default 0.262
INPUT float Center_radius;   // default 21.

INPUT float Effect_intensity;

void main()
{
    vec2 uv = _uv;
    uv *= 1.0 - uv.yx;
    uv.x *= _aspect_ratio;

    vec3 in_color = image(_uv);

    vec3 color = vignetting(
        in_color, Effect_intensity, uv,
        Border_darkness, Center_radius
    );

    out_Color = vec4(color, 1.);
}