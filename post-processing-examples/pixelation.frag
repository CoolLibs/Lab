#version 430

//#include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/define_types.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/pixelation.glsl"

input float Nb_tiles_on_y_axis; // default 20
input float Border;             // default 0.469
input float Smoothing;          // default 0.225

input RgbColor Border_color;

input float Effect_intensity;

void main()
{
    vec2 uv = _uv;
    uv.x *= _aspect_ratio;

    RgbColor in_color = image(_uv);

    RgbColor out_color = pixelation(
        in_color, Effect_intensity, uv,
        Nb_tiles_on_y_axis, Border, Smoothing, Border_color
    );

    out_Color = vec4(out_color, 1.);
}