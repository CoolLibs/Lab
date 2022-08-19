#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/flare.glsl"

INPUT float x;
INPUT float y;
INPUT float Brightness;

INPUT int Nb_of_circles; // default 10
INPUT int Seed;          // default 20

INPUT float Effect_intensity;

void main()
{
    vec3 in_color = image(_uv);

    vec3 out_color = flare(
        in_color, Effect_intensity,
        x, y, Brightness, Nb_of_circles, Seed
    );

    out_Color = vec4(out_color, 1.);
}