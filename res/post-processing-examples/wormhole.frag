#version 410

// #include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/wormhole.glsl"

INPUT float Atractivity; // default 0.008
INPUT float Symmetry;    // min = 0 max = 0.5
INPUT float Intensity;
INPUT float Wormhole_smooth; // fobidden 0
INPUT float Wormhole_size;   // fobidden 0
INPUT vec2  Center;

INPUT RgbColor Wormhole_color;

INPUT float Effect_intensity;

void main()
{
    vec2 in_uv = _uv;
    vec3 color = wormhole(
        in_uv, Effect_intensity,
        _aspect_ratio,
        Center, Atractivity, Symmetry, Intensity, Wormhole_smooth, Wormhole_size, Wormhole_color
    );

    out_Color = vec4(color, 1.);
}
