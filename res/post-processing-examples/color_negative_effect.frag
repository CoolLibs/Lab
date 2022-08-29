#version 410

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"

// default 1 min = 0
INPUT vec3 channels_contribution;

void main()
{
    vec2 uv    = _uv;
    vec3 color = vec3(1.);
    vec3 img   = image(uv);
    color -= channels_contribution * img;
    out_Color = vec4(color, 1.);
}