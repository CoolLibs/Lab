#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/define_types.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/golden_noise.glsl"

// BEGIN DYNAMIC PARAMS

uniform float Seed; // default 48

uniform float Effect_intensity;

// END DYNAMIC PARAMS

void main()
{
    RgbColor image = image(_uv);
    vec2     uv    = _uv;
    uv.x *= _aspect_ratio;

    RgbColor out_color = RgbColor(
        golden_noise(uv, Seed + 0.1),
        golden_noise(uv, Seed + 0.2),
        golden_noise(uv, Seed + 0.3)
    );

    out_Color = vec4(out_color, 1.);
}