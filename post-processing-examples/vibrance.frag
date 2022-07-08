#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_ROOT_FOLDER_/shader-lib/image.glsl"
// #include "_ROOT_FOLDER_/shader-lib/vibrance.glsl"

// BEGIN DYNAMIC PARAMS

uniform float Vibrance; // default 0.

uniform float Effect_intensity;

// END DYNAMIC PARAMS

void main()
{
    vec3 in_color = image(_uv);

    vec3 out_color = vibrance(
        in_color, Effect_intensity,
        Vibrance
    );

    out_Color = vec4(out_color, 1.);
}