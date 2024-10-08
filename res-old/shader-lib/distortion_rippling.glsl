// https://www.shadertoy.com/view/MlsyzB

vec2 distortion_rippling(
    vec2 in_uv, float mask,
    vec2 center, float radius, float thickness
)
{
    vec2  uv            = in_uv - center;
    float d             = length(uv);
    float radius_effect = radius * 0.25;
    float distort       = cos(((d - (radius_effect - thickness)) / thickness) * (PI / 2.0));

    // Try adding any of these for some extra flavor
    // distort /= cos(distort); 			// 1
    // distort /= distort * cos(distort);	// 2
    // distort /= sin(distort);				// 3
    // distort /= distort * sin(distort);	// 4
    // distort += distort;					// 5
    return mix(in_uv, (uv * distort) + center, mask);
}