#version 410

// #include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"

INPUT Angle angle_in_turns; // 0 to 1 (1 == 1 turn)
INPUT float hole_radius;    // positive values
INPUT float hole_intensity; // 0 to 8
// INPUT float hole_attractivity; // -1 to 1
INPUT float hole_glow; // -.5 to 1
// range 0 to 1
// Point 2D
INPUT vec2 hole_center; // default vec2(0.5,0.5)

INPUT RgbColor hole_color;

INPUT float Effect_intensity;

vec3 hole(vec2 point, vec2 uv, vec3 color, float invIntensity, float invGlow)
{
    float d    = distance(uv, point);
    float invD = 1.0 / d;
    invD       = pow(invD / invIntensity, invGlow);
    return color * invD;
}

void main()
{
    vec2 ncoord = _uv - hole_center;

    // sink hole projection
    float angle  = atan(ncoord.x, ncoord.y);
    float radius = length(ncoord);
    vec2  uv     = vec2(angle, radius);

    uv.y += Effect_intensity;

    vec3 col = image(mix(_uv, uv, Effect_intensity));

    // hole
    // float invIntensity = (6.0 + sin(radians(hole_intensity * 40)) * 5.8) / (hole_radius);
    // vec3  out_col      = col + hole(vec2(0., 0.), ncoord, -hole_color, invIntensity, hole_glow);

    // out_col   = mix(col, out_col, Effect_intensity);
    out_Color = vec4(col, 1.);
}