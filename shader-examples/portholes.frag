#version 430

//#include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform float Square_mode; // bool
uniform float Size;        // 10
uniform float Space_to_border;
uniform float Speed;
uniform float Movement;
uniform float Time_mode;      // bool
uniform float Distance_mode;  // 2
uniform float Zoom_intensity; // 0.001
uniform float Translation_On_Off;

uniform float Change_center;
uniform float Border;
uniform float Smoothing;
uniform float Border_smoothing;

uniform vec3 Border_color;

// END DYNAMIC PARAMS

void main()
{
    vec2 uv = _uv;
    uv.x *= _aspect_ratio;

    vec3 in_color = image(_uv);

    vec3 out_color = portholes(
        in_color, Effect_intensity, uv,
        Square_mode, Size, Space_to_border, Speed, Movement, Time_mode, Distance_mode, Zoom_intensity, Translation_On_Off,
        Change_center, Border, Smoothing, Border_smoothing, Border_color
    );

    out_Color = vec4(out_color, 1.);
}