#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/define_types.glsl"
// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/flare.glsl"

input float x;
input float y;
input float Brightness;

input int Nb_of_circles; // default 10
input int Seed;          // default 20

input float Effect_intensity;

void main()
{
    RgbColor in_color = image(_uv);

    RgbColor out_color = flare(
        in_color, Effect_intensity,
        x, y, Brightness, Nb_of_circles, Seed
    );

    out_Color = vec4(out_color, 1.);
}