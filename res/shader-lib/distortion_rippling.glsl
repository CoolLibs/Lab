// https://www.shadertoy.com/view/MlsyzB

vec2 distortion_rippling(
    vec2 in_uv, float Effect_intensity,
    vec2 Center, float Radius, float Thickness
)
{
    vec2  uv      = in_uv - Center;
    float d       = length(uv);
    float radius  = Radius * 0.25;
    float distort = cos(((d - (radius - Thickness)) / Thickness) * (PI / 2.0));

    // Try adding any of these for some extra flavor
    // distort /= cos(distort); 			// 1
    // distort /= distort * cos(distort);	// 2
    // distort /= sin(distort);				// 3
    // distort /= distort * sin(distort);	// 4
    // distort += distort;					// 5
    return mix(in_uv, (uv * distort) + Center, Effect_intensity);
}