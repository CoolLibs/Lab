#version 410

// https://www.shadertoy.com/view/MtKGzt

// #include "_COOL_RES_/shaders/math.glsl"

layout(location = 0) in vec2 _uv;
uniform float _time;
uniform float _aspect_ratio;
out vec4      out_Color;

uniform sampler2D _image;

// #include "_COOL_RES_/shaders/input_definitions.glsl"
// #include "_ROOT_FOLDER_/res/shader-lib/image.glsl"

INPUT Angle Angle_in_turns;    // 0 to 1 (1 == 1 turn)
INPUT float Hole_radius;       // positive values
INPUT float Hole_intensity;    // 0 to 8
INPUT float Hole_attractivity; // -1 to 1
INPUT float Hole_smooth;       // -.5 to 1
// range 0 to 1
// Point2D
INPUT vec2 Hole_center; // vec2(0.5,0.5)

INPUT RgbColor Hole_color;

INPUT int Subdivision; // 0 forbidden

vec3 Hole(vec2 point, vec2 uv, vec3 color, float invIntensity, float invGlow)
{
    float d    = distance(uv, point);
    float invD = 1.0 / d;
    invD       = pow(invD / invIntensity, invGlow);
    return color * invD;
}

void main()
{
    vec2 coord  = (_uv - Hole_center) * 2.;
    vec2 ncoord = coord * vec2(_aspect_ratio, 1.0);

    // rotation
    float angleRot = Angle_in_turns * TAU;
    vec2  tcoord   = vec2(ncoord.x * cos(angleRot) - ncoord.y * sin(angleRot), ncoord.y * cos(angleRot) + ncoord.x * sin(angleRot));

    // sink Hole projection
    float angle  = atan(tcoord.x, tcoord.y);
    float radius = length(tcoord);
    vec2  uv     = vec2(angle, radius);

    uv /= radians(360. / float(Subdivision));
    uv.y += Hole_attractivity;

    vec4 col = texture(_image, uv);

    // Hole
    float invIntensity = (6.0 + sin(radians(Hole_intensity * 40)) * 5.8) / (Hole_radius);
    float invGlow      = Hole_smooth;
    col.rgb += Hole(vec2(0, 0), tcoord, -Hole_color, invIntensity, invGlow);

    out_Color = col;
}