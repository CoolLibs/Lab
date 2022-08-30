#version 410

//#include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/portholes.glsl"

INPUT bool  Square_mode; // bool
INPUT float Size;        // 10
INPUT float Space_to_border;
INPUT float Speed;
INPUT float Movement;
INPUT bool  Time_mode;      // bool
INPUT float Distance_mode;  // 2
INPUT float Zoom_intensity; // 0.001

INPUT float Change_center;
INPUT float Border;
INPUT float Smoothing;
INPUT float Border_smoothing;
INPUT float Effect_intensity;

INPUT RgbColor Border_color;

void main()
{
    vec2 uv = _uv;

    vec3 in_color = image(_uv);

    vec3 out_color = portholes(
        in_color, Effect_intensity, uv,
        Square_mode, Size, Space_to_border, Speed, Movement, Time_mode, Distance_mode, Zoom_intensity,
        Change_center, Border, Smoothing, Border_smoothing, Border_color
    );

    out_Color = vec4(out_color, 1.);
}