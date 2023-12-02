#version 430

in vec2  _varying_uv;
out vec4 _out_color;

void main()
{
    // if (length(vTexCoords - vec2(0.5)) > 0.5)
    // discard;
    _out_color = vec4(vec3(1.), 1.);
}