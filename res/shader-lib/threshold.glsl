float threshold(float x, int thres)
{
    x = clamp(x, 0., 0.999); // Make sure that pure white doesn't get mapped to a range of its own by the floor()
    x *= (thres);
    x = floor(x);
    x /= (thres - 1);
    return x;
}