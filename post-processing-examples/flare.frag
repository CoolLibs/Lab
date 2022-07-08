#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/flare.glsl"

// BEGIN DYNAMIC PARAMS

uniform float x;
uniform float y;
uniform float Brightness;

uniform int Nb_of_circles; // default 10
uniform int Seed;          // default 20

uniform float Effect_intensity;

// END DYNAMIC PARAMS

void main()
{
    vec3 in_color = image(_uv);

    vec3 out_color = flare(
        in_color, Effect_intensity,
        x, y, Brightness, Nb_of_circles, Seed
    );

    out_Color = vec4(out_color, 1.);
}