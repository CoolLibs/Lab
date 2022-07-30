#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/bichrome.glsl"

INPUT RgbColor Color1;           // default (0.5, 0.5, 0.5) hdr
INPUT RgbColor Color2;           // hdr default (0.3, 0.6, 0.2)
INPUT float    Effect_intensity; // default 1 min -1 max 2

void main()
{
    vec3 in_color = image(_uv);

    vec3 out_color = bichrome(
        in_color, Effect_intensity,
        Color1, Color2
    );

    out_Color = vec4(out_color, 1.);
}