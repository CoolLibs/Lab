#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/define_types.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/vignetting.glsl"

// BEGIN DYNAMIC PARAMS

uniform float Border_darkness; // Default 0.262
uniform float Center_radius;   // default 21.

uniform float Effect_intensity;

// END DYNAMIC PARAMS

void main()
{
    vec2 uv = _uv;
    uv *= 1.0 - uv.yx;
    uv.x *= _aspect_ratio;

    RgbColor in_color = image(_uv);

    RgbColor out_color = vignetting(
        in_color, Effect_intensity, uv,
        Border_darkness, Center_radius
    );

    out_Color = vec4(out_color, 1.);
}