
You can define other functions in you *.clbnode* file, and call them in your `main` function as usual. Only use regular glsl types in these functions (int, float, vec2, vec3, vec4, ...). They are allowed to have as many inputs of has many types as you want.<br/>
You are allowed to use your INPUTs inside these functions.

```glsl
INPUT float `Value`;

float helper(float x, bool maybe)
{
    return x + (maybe ? `Value` : 5.);
}

float main(float x)
{
    return helper(x, x > 2.);
}
```