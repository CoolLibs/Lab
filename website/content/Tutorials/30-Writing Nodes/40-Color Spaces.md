
There exist quite a few different color spaces and alpha spaces. They all have a reason to be and address a different aspect of the complex problem that is color representation. Understanding the differences between these spaces is essential to determine which space will be better suited for the node you want to implement.

## Color Spaces

You can watch this presentation to get of good first introduction to color spaces: [RGB to XYZ: The Science and History of Color](https://youtu.be/AS1OHMW873s).

If you have the choice between several color spaces because your effect will work the same in any of those, here is a list in order of which to prefer:
- CIELAB
- Linear RGB
- sRGB
- HSLuv

Now let's see why you might prefer one space over another:

### CIELAB

In this space the euclidean distance between the `vec3`s accurately represents the perceived difference in color by a human. This means that if you need to do anything that is perceptually accurate, e.g. blending between two colors, CIELAB is what you need.<br/>
**This is the space that most effects will want to use.**

The first component stores the lightness, a number between 0 and 1.<br/>
The second and third components store chromaticity information. They don't have a specific range.

### HSLuv

You might already be familiar with HSL (Hue - Saturation - Lightness); [HSLuv](https://www.hsluv.org/) is simply a perceptually-accurate version of it.

**This is what you will want to use in nodes that directly manipulate either the hue and / or the saturation.** (Note that CIELAB also gives you access to the lightness so there is no need to use HSLuv if you are only interested in the lightness).

The three channels are numbers between 0 and 1.

### Linear RGB

When doing physically-based rendering in 3D, you are using Linear RGB because you are manipulating amounts of light (photons) and adding them together.<br/>
**This is why this space will be the one that a 3D renderer will manipulate and output.**

All the three channels are numbers between 0 and 1.

### sRGB

This is the standard input and output format. All the colors in our UI are expressed in sRGB, and all the images you will import most likely contain sRGB values. If you go to any color picker / color palette website, they will give you values in sRGB.<br/>
**Effects hardly ever need to manipulate sRGB though**, because this space is neither perceptually nor physically correct.

All the three channels are numbers between 0 and 1.

## Alpha Spaces

### Premultiplied

Premultiplied alpha is when the color components have been multiplied by the alpha. This might sound like a surprising thing to do, but it actually helps effects like blurring and blending to behave properly. [See this great lesson](https://youtu.be/WtYfF48Z9mA?list=PL9_jI1bdZmz2emSh0UQ5iOdT2xRHFHL7E&t=960).<br/>
**This is why premultiplied alpha will almost always be what you want to use.**


> NB: for Color->Color effects that don't use the alpha, if some premultiplied alpha is plugged in as input we first un-premultiply the color, then pass it to the effect, then re-premultiply. This is the same behavior as ["Pre-Divide / Post-Multiply" in Resolve/Fusion](https://youtu.be/klqSJiPqmGU). This basically means that everything will work as expected intuitively and you don't have to worry about it.

### Straight

Straight alpha is what most people know. This is what is used in our UI and what any imported image will most likely use. **Effects will hardly ever need to use it though** because it doesn't work quite as good as premultiplied alpha.