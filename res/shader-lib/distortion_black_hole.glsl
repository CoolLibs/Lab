// https://www.shadertoy.com/view/MtKGzt

vec3 Hole(vec2 point, vec2 uv, vec3 color, float invIntensity, float invGlow)
{
    float d    = distance(uv, point);
    float invD = 1.0 / d;
    invD       = pow(invD / invIntensity, invGlow);
    return color * invD;
}

vec3 distortion_black_hole(
    vec2 in_uv, float Effect_intensity,
    vec2 Hole_center, Angle Angle_in_turns, RgbColor Hole_color, int Subdivision,
    float Hole_radius, float Hole_intensity, float Hole_attractivity, float Hole_smooth

)
{
    vec2 coord  = (in_uv - Hole_center) * 2.;
    vec2 ncoord = coord;

    // rotation
    float angleRot = Angle_in_turns * TAU;
    vec2  tcoord   = vec2(ncoord.x * cos(angleRot) - ncoord.y * sin(angleRot), ncoord.y * cos(angleRot) + ncoord.x * sin(angleRot));

    // sink Hole projection
    float angle  = atan(tcoord.x, tcoord.y);
    float radius = length(tcoord);
    vec2  uv     = vec2(angle, radius);

    uv /= radians(360. / float(Subdivision));
    uv.y += Hole_attractivity;

    vec3 out_color = image(mix(_uv, uv, Effect_intensity));

    // Hole
    float invIntensity = (6.0 + sin(radians(Hole_intensity * 40)) * 5.8) / (Hole_radius);
    float invGlow      = Hole_smooth;
    out_color += Hole(vec2(0, 0), tcoord, -Hole_color, invIntensity, invGlow) * Effect_intensity;

    return out_color;
}