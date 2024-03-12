
## Functions

You can define other functions in you *.clbnode* file, and call them in your `main` function as usual. Only use regular glsl types in these functions (int, float, vec2, vec3, vec4, ...). They are allowed to have as many inputs of as many types as you want.<br/>
You are allowed to use your INPUTs inside these functions.

```glsl
INPUT float 'Value';

float helper(float x, bool maybe)
{
    return x + (maybe ? 'Value' : 5.);
}

float main(float x)
{
    return helper(x, x > 2.);
}
```

## Files

You can also create your own helper files and `#include` them in your nodes. This can be useful if you want to reuse a big chunk of code in several nodes. To do so, simply create your file in the *Nodes* folder and then in your node:
```glsl
#include "_USER_DATA_/Nodes/my_file.glsl"
```

NB: `_USER_DATA_` is a special keyword that refers to the user-data folder.