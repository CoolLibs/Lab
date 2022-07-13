#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/define_types.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/contrast_saturation_brightness.glsl"

// BEGIN DYNAMIC PARAMS

uniform float Contrast;   // default 1.0
uniform float Brightness; // default 1.0
uniform float Saturation; // default 1.0

uniform float Effect_intensity;

// END DYNAMIC PARAMS

void main()
{
    RgbColor in_color = image(_uv);

    RgbColor out_color = contrast_saturation_brightness(
        in_color, Effect_intensity,
        Contrast, Brightness, Saturation
    );

    out_Color = vec4(out_color, 1.);
}