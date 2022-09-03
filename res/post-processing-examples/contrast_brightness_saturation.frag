#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/contrast_brightness_saturation.glsl"

// TODO(ASG) Make sure a value of 0 means no effect, and make 0 the default
INPUT float Contrast;   // default 1.0
INPUT float Brightness; // default 1.0
INPUT float Saturation; // default 1.0

INPUT float Effect_intensity;

void main()
{
    vec3 in_color = image(_uv);

    vec3 color = contrast_brightness_saturation(
        in_color, Effect_intensity,
        Contrast, Brightness, Saturation
    );

    out_Color = vec4(color, 1.);
}