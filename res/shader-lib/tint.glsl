vec3 tint(
    vec3 in_color, float mask,
    float tint
)
{
    const vec3 kRGBToYPrime = vec3(0.299, 0.587, 0.114);
    const vec3 kRGBToI      = vec3(0.596, -0.275, -0.321);
    const vec3 kRGBToQ      = vec3(0.212, -0.523, 0.311);

    const vec3 kYIQToR = vec3(1.0, 0.956, 0.621);
    const vec3 kYIQToG = vec3(1.0, -0.272, -0.647);
    const vec3 kYIQToB = vec3(1.0, -1.107, 1.704);

    float YPrime      = dot(in_color, kRGBToYPrime);
    float I           = dot(in_color, kRGBToI);
    float Q           = dot(in_color, kRGBToQ);
    float tint_adjust = atan(Q, I);
    float chroma      = sqrt(I * I + Q * Q);

    tint_adjust += tint;

    Q = chroma * sin(tint_adjust);
    I = chroma * cos(tint_adjust);

    vec3 yIQ = vec3(YPrime, I, Q);

    vec3 out_color = vec3(dot(yIQ, kYIQToR), dot(yIQ, kYIQToG), dot(yIQ, kYIQToB));

    return mix(in_color, out_color, mask);
}