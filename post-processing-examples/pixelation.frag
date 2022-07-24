#version 410

//#include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/pixelation.glsl"

INPUT float Nb_tiles_on_y_axis; // default 20
INPUT float Border;             // default 0.469
INPUT float Smoothing;          // default 0.225

INPUT RgbColor Border_color;

INPUT float Effect_intensity;

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