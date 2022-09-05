float mix_hues(float a, float b, float t)
{
    float m = min(a, b);
    float M = max(a, b);
    if (M - m < m + 1 - M)
    {
        return mix(a, b, t);
    }
    else
    {
        if (a < b)
        {
            return fract(mix(a + 1., b, t));
        }
        else
        {
            return fract(mix(a, b + 1., t));
        }
    }
}
