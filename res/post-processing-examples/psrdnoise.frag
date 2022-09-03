#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/fbm.glsl"

INPUT float Scale;
INPUT float Gain;       // default = 0.5
INPUT float Lacunarity; // default =2
INPUT int   Octaves;    // min=1

INPUT float Effect_intensity; // default 1

void main()
{
    vec2 in_uv = _uv;

    vec2 out_uv = fbm(
        in_uv, Effect_intensity,
        Scale, Lacunarity, Gain, Octaves
    );

    vec3 out_color = image(out_uv);

    out_Color = vec4(out_color, 1.);
}