
CoolLab nodes are tiny fragments of shader code written in glsl. If you don't know what that is you can start by [watching tutorials from The Art of Code](https://youtu.be/u5HAYVHsasc).

To create a new node you need to add a new *.clbnode* file in the *Nodes* folder that is next to your CoolLab executable. The *Nodes* folder contains several folders representing the different categories of nodes. You can create a new category by simply creating a new folder, but most likely your node will already belong to one of the existing categories.

Here is an example of a very basic node:
```glsl title="Invert Color.clbnode"
sRGB main(sRGB Color)
{
    return vec3(1.) - Color;
}
```

All nodes need to have a function called `main`: this is what will be used to apply the effect on whichever node will be plugged into your node.<br/>
In CoolLab a lot of implicit conversions and code generation happen behind the scene. This means that you can write minimal nodes and the rest will happen automagically. For example our `Invert Color` node takes a color and outputs a new color. If you plug a whole image into the `Invert Color` node, the `main` function you defined will be called on each pixel of the image, thus creating a new image with inverted colors. Notice that in our `Invert Color` node we never mentioned images, only colors, which is a simpler and smaller building block. This is great! Our nodes stay very simple and focused on what they want to achieve.

It is possible for the `main` function to have several inputs, *but they must all be of the same type*:[^1]

```glsl title="Add.clbnode"
float main(float x, float y)
{
    return x + y;
}
```

[^1]: If you have a use case where this limitation is annoying to you, feel free to [contact us](https://github.com/CoolLibs/Lab/issues/new?assignees=&labels=enhancement%2Ctriage&template=feature-suggestion.yaml&title=%5BFeature%5D+) and we will happily consider your use case and improve CoolLab to make it fit your needs.

Also, it is possible that your `main` function doesn't take any input at all if it represents a constant:

```glsl title="Float Value.clbnode"
INPUT float `Value`;

float main()
{
    return `Value`;
}
```