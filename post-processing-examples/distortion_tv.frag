#version 410

// https://www.shadertoy.com/view/XtlXzj

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"

INPUT float time_mod;
INPUT float strip; // min = 1

INPUT int nb_iteration; // min = 1 max 23

vec3 draw(vec2 uv)
{
    return vec3(texture(_image, vec2(uv.x, uv.y)).rgb);
}

float terrain(float x)
{
    // Used Shadershop
    // return ((sin( (sin( (x - -1.33) / 0.76 / 1.23 ) * 0.8 - 0.69) / 0.58 )) * (((((sin( (((x - -1.33) / 0.76 - -3.0) / 2.61 - -0.38) / 1.52 ) * 2.25) * (sin( (((x - -1.33) / 0.76 - -3.0) / 2.61 - -0.47) / 1.61 ) * 1.03))) * (sin( ((x - -1.33) / 0.76 - -3.0) / 2.61 / 0.44 ) * 1.48)) * 1.08)) * 0.78;
    // You actually should look after sigma.
    float v = 0.;
    x *= 3.;
    for (float n = 0.; n < nb_iteration; n++)
    {
        v += ((sin((x * sin(n / 2.142)) + (n / 1.41))) / nb_iteration) * strip;
    }
    return pow(v, 3.);
}

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

void main()
{
    float time         = time_mod;
    vec2  uv           = _uv;
    vec3  dist_texture = vec3(draw(uv + (terrain((uv.y * 20.) + (time * 30.)) / 200.)).r, draw(uv + (terrain((uv.y * 22.) + (time * 30.)) / 201.)).g, draw(uv + (terrain((uv.y * 14.) + (time * 30.)) / 202.)).b);
    out_Color          = vec4(dist_texture, 1.0);
}