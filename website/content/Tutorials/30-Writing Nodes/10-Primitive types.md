
## Types

Now that you know how to write a basic node, you might wonder what all the possibilities are. Here are the types you will use most often as inputs and outputs of your `main` function:

- `UV`: The 2D coordinates that we use to sample our images. (`vec2` under the hood).
- `float`: Just a regular number.
- `SignedDistance`: A distance to an object, used for [SDF rendering](https://youtu.be/PGtv-dBi2wE). (`float` under the hood).
- Color (`Oklab`, `sRGB`, `LinearRGB` or `Okhsl`). See the [Color Spaces section](40-Color%20Spaces.md) to learn all about the various color types. (`vec3` under the hood).
- Color and alpha. (`Oklab_PremultipliedA`, `Oklab_StraightA`, `LinearRGB_PremultipliedA`, `LinearRGB_StraightA`, `sRGB_PremultipliedA`, `sRGB_StraightA`, `Okhsl_PremultipliedA` or `Okhsl_StraightA`). See the [Color Spaces section](40-Color%20Spaces.md) to learn all about the various color types. (`vec4` under the hood).

And here are all the remaining types, that are seldom used:
- `bool`
- `int`
- `vec2`
- `vec3`
- `vec4`
- `mat2`
- `mat3`
- `mat4`
- `Angle` (`float` under the hood).
- `Hue` (`float` under the hood).
- `Direction2D` (`vec2` under the hood, guaranteed to be of length 1).

:::info
It is important to note that these types are purely semantic, and in your code you can just treat them as regular int / float / vec2 / vec3 / vec4.
:::

## Examples

Here are a few examples of typical `main` functions you might define:

```glsl title="Color Transformation"
Oklab main(Oklab Color)
{
    // ...
}
```

```glsl title="2D Modifier (Zoom, Distortion, ...)"
UV main(UV Input)
{
    // ...
}
```

```glsl title="Image"
Oklab main(UV uv)
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
Oklab_PremultipliedA main(Oklab_PremultipliedA Over, Oklab_PremultipliedA Under)
{
    // ...
}
```

```glsl title="Black & White"
float main(Oklab Color)
{
    // ...
}
```

```glsl title="Colorizer"
Oklab main(float Greyscale)
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