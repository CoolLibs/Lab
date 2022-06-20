#version 430

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// BEGIN DYNAMIC PARAMS

uniform vec3 Color_shadows;
uniform vec3 Color_highlights;

uniform float highlights_intensity;
uniform float shadows_intensity;

// END DYNAMIC PARAMS

vec4 image(vec2 uv)
{
    return texture2D(_image, uv);
}

vec3 desaturate(vec3 color, float amount)
{
    const vec3 luminosity_coefficient = vec3(0.2125, 0.7154, 0.0721);
    float      greyscale              = dot(color, luminosity_coefficient);
    return mix(color, vec3(greyscale), amount);
}

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float shadows_detection(vec3 image)
{
    float hue_pixel = rgb2hsv(image).z;
    return smoothstep(.9, 0.3, hue_pixel);
}

float highlights_detection(vec3 image)
{
    float hue_pixel = rgb2hsv(image).z;
    return smoothstep(.9, 1., hue_pixel);
}

void main()
{
    vec3  color      = image(_uv).rgb;
    float shadows    = shadows_detection(color);
    float highlights = highlights_detection(color);
    color            = mix(mix(color, Color_highlights, highlights * highlights_intensity), Color_shadows, shadows * shadows_intensity);
    out_Color        = vec4(color, 1.);
}