
## Types

Now that you know how to write a basic node, you might wonder what all the possibilities are. Here are the types you will use most often as inputs and outputs of your `main` function:

- `UV`: The 2D coordinates that we use to sample our images. (`vec2` under the hood).
- `float`: Just a regular number.
- `SignedDistance`: A distance to an object, used for [SDF rendering](https://youtu.be/PGtv-dBi2wE). (`float` under the hood).
- Color (`CIELAB`, `sRGB`, `LinearRGB` or `HSLuv`). See the [Color Spaces section](40-Color%20Spaces.md) to learn all about the various color types. (`vec3` under the hood).
- Color and alpha. (`CIELAB_PremultipliedA`, `CIELAB_StraightA`, `LinearRGB_PremultipliedA`, `LinearRGB_StraightA`, `sRGB_PremultipliedA`, `sRGB_StraightA`, `HSLuv_PremultipliedA` or `HSLuv_StraightA`). See the [Color Spaces section](40-Color%20Spaces.md) to learn all about the various color types. (`vec4` under the hood).

And here are all the remaining types, that are seldom used:
- `int`
- `vec2`
- `vec3`
- `vec4`
- `Angle` (`float` under the hood).
- `Direction2D` (`vec2` under the hood, guaranteed to be of length 1).

:::info
It is important to note that these types are purely semantic, and in your code you can just treat them as regular int / float / vec2 / vec3 / vec4.
:::

## Examples

Here are a few examples of typical `main` functions you might define:

```glsl title="Color Transformation"
CIELAB main(CIELAB Color)
{
    // ...
}
```

```glsl title="Space Transformation (Zoom, Distortion, ...)"
UV main(UV Input)
{
    // ...
}
```

```glsl title="Image"
CIELAB main(UV uv)
{
    // ...
}
```

```glsl title="Math function"
float main(float x)
{
    // ...
}
```

```glsl title="Parametric curve"
UV main(float Percentage)
{
    // ...
}
```

```glsl title="Mask"
float main(UV uv)
{
    // ...
}
```

```glsl title="Blend Mode"
CIELAB_PremultipliedA main(CIELAB_PremultipliedA Over, CIELAB_PremultipliedA Under)
{
    // ...
}
```

```glsl title="Black & White"
float main(CIELAB Color)
{
    // ...
}
```

```glsl title="Colorizer"
CIELAB main(float Greyscale)
{
    // ...
}
```

```glsl title="SDF 2D"
SignedDistance main(UV uv)
{
    // ...
}
```

```glsl title="SDF 3D"
SignedDistance main(vec3 pos)
{
    // ...
}
```