#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/fbm.glsl"

// BEGIN DYNAMIC PARAMS

uniform float Scale;
uniform float Gain;       // default = 0.5
uniform float Lacunarity; // default =2

uniform int Octaves; // min=1

uniform float Effect_intensity; // default 1

// END DYNAMIC PARAMS

void main()
{
    vec2 uv = _uv;

    vec2 uv_noise = fbm(
        Effect_intensity, uv,
        Scale, Lacunarity, Gain, Octaves
    );
    vec3 out_color = image(uv_noise);
    out_Color      = vec4(out_color, 1.);
}