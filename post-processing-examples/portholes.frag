#version 410

//#include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/portholes.glsl"

input bool  Square_mode; // bool
input float Size;        // 10
input float Space_to_border;
input float Speed;
input float Movement;
input bool  Time_mode;      // bool
input float Distance_mode;  // 2
input float Zoom_intensity; // 0.001

input float Change_center;
input float Border;
input float Smoothing;
input float Border_smoothing;

input RgbColor Border_color;

void main()
{
    vec2 uv = _uv;
    uv.x *= _aspect_ratio;

    RgbColor in_color = image(_uv);

    RgbColor out_color = portholes(
        in_color, Effect_intensity, uv,
        Square_mode, Size, Space_to_border, Speed, Movement, Time_mode, Distance_mode, Zoom_intensity,
        Change_center, Border, Smoothing, Border_smoothing, Border_color
    );

    out_Color = vec4(out_color, 1.);
}