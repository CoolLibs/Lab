#version 410

uniform float _time;
out vec4      out_Color;
#include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"

INPUT float Blue;

void main()
{
    vec2 uv    = normalized_uv();
    vec3 color = vec3(0.);

    color = vec3(uv, Blue);

    out_Color = vec4(color, 1.);
}