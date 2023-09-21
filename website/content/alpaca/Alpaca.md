https://linktr.ee/coollab_art

Coollab is a node-based generative-art software where you compose small building blocks to create abstract artworks. It shares a few similarities with TouchDesigner, NotchVFX and the likes. But unlike any software that we are aware of, Coollab takes a much higher-level approach, inspired by functional programming. The main design guideline of Coollab is to have a very low barrier to entry, and make shader-art techniques accessible even to people with little to no knowledge in programming or mathematics. While at the same time making it very easy for people who know shader programming to add their own effects and nodes to the software.

# Simplifying node-based programming by raising the level of abstraction

## Intro

Disclaimer: we are going to describe the approach we took for an image-generation software. We do not know whether the same approach would work equally well in other contexts. For future work, we are going to try and apply the same principles to a graph for particle systems. Even though we are not convinced the benefits will be as clear, we think this is an avenue worth exploring, and we believe our approach should be considered as a possibility whenever designing any node-based system.

## Demo

Here is an example of an artwork made with Coollab:
[TODO refaire cette image en aillant les bons noms d'inputs sur les nodes + avoir le dernier node en main node]
![Alt text](<demo V1.png>)
![Alt text](images/img(0).png)

As you can see, the graph is very declarative: you start with a shape (`Disk`) and then apply several effects on top of it (`Kaleidoscope`, `Zoom`, etc.).

What allows us to be that declarative is that we do not pass *data* between nodes, but *functions*. Each node receives a function (the previous node) and decides when to call it, with which parameters, and what to do with the output. This allows us to have more freedom when implementing nodes, and abstract more details away from the user.

We will come back to what this *function vs data* means, but first you need to understand the basics of what Coollab builds upon: shader art.

## Intro to shaders

If you are not familiar with shader art, you can watch this great introduction: [An introduction to Shader Art Coding, *by kishimisu*](https://youtu.be/f4s1h2YETNY). But in essence the only thing you need to know is that we want to write some code that will be run for each pixel of our image, and will compute a color based solely on the position (called UV) of the current pixel. For example, here is how you would draw a Disk:
```glsl title="Disk"
function Disk(uv: UV) -> Color
{
    if(distance(uv, vec2(0, 0)) < 1) // If the pixel is inside the disk (distance from the origin less than 1)
        return vec3(1.);             // Then color the pixel white
    else
        return vec3(0.);             // Otherwise color it black
}
```
After that function has been run on all the pixels, some of them will have been colored white, others black, and overall they will form a white disk on a black background.

In Coollab, users don't need to write code (although advanced users can), they can use the high-level nodes that we provide: they wrap-up common operations you might want to do in code, and Coollab generates the shader code under the hood based on the nodes and links you created.

## Function-flow vs Data-flow

What makes Coollab so easy to use is that the links between nodes do not convey data, but functions, which means users never see the nitty-gritty implementation details of the effects, and can use them in a declarative way, without worrying about the order of the data-processing or anything of that nature.

![](./grid_of_disks.png)
For example, if we want to make a grid of disks, in shader we first need to have a function that creates a disk, and then modify it's input to make it tile by using a `fract` (cf. https://youtu.be/svLzmFuSBhk?t=235, where `mod(x, 1)` is equivalent to `fract(x)`).

So a data-oriented workflow like we can find in Blender's Shader Nodes or Unity's Shader Graph would look like this: we first get the position (UV), then modify it with a Grid node, then pass it to the Disk node.

![Alt text](./data_flow.png)

And the implementation of the nodes would look something like this:

```glsl title="Get UV"
function GetUV() -> UV
{
    return uniform_uv; // Global variable accessible to the shader
}
```
```glsl title="Grid"
function Grid(uv: UV) -> UV
{
    return fract(uv); // Modify the UV
}
```
```glsl title="Disk"
function Disk(uv: UV) -> Color
{
    if(length(uv) < 1)
        return vec3(1.); // Color the pixel white
    else
        return vec3(0.); // Color the pixel black
}
```

On the other hand, in Coollab you first create your Disk, and then pass that *function* to the Grid node, which will call it with a modified position. The pins don't represent UVs or Colors, but Images (aka functions from UV to Color).

![Alt text](function_flow.png)

```glsl title="Disk"
function Disk(uv: UV) -> Color
{
    if(length(uv) < 1)
        return vec3(1.); // Color the pixel white
    else
        return vec3(0.); // Color the pixel black
}
```
*NB: The Disk node hasn't changed, but it will be used differently: instead of calling it with some uv, we will pass the entire function defined there to the next node (a.k.a. Grid), which will decide when and how to call it.*
```glsl title="Grid"
INPUT function Image: UV -> Color; // Declare an input pin on the node, that will receive a function called Image.

function Grid(uv: UV) -> Color
{
    return Image(fract(uv)); // Use the "Image" function with a modified input (UV).
}
```
*NB: this implementation of the Grid might seem more cumbersome to write than the data-flow one, but actually in Coollab we can generate this code from the simpler data-oriented one that you saw earlier, so you get the best of both worlds, and writing nodes remains very easy.*
*NB: the code has been slightly simplified for the purpose of this article. If you want to see what our actual nodes look like, you can [find them on our GitHub](https://github.com/CoolLibs/Lab/blob/main/Nodes/30%20Shape/Disk.clbnode).*

To understand why this approach can be more intuitive, let's consider another effect, this time modifying the *Color* instead of the *UV* (a.k.a. the *output* of the Disk function instead of its *input*).

```glsl title="Brighten"
function Brighten(color: Color) -> Color
{
    return color * 2; // Modify the color
}
```
It would be used like this:
![Alt text](data_flow2.png)

Whereas in Coollab the effect would be written as
```glsl title="Brighten"
INPUT function Image: UV -> Color; // Declare an input pin on the node, that will receive a function called Image.

function Brighten(uv: UV) -> Color
{
    return Image(uv) * 2; // Use the "Image" function with a modified output.
}
```
and be used like so:
![Alt text](function_flow2.png)

Note that in Coollab, all effects are applied *after* the node they apply to (we either take a Disk and repeat it in a Grid, or take a Disk and Brighten it). Whereas with a data-oriented approach some effects need to be applied *before* and others *after*! (And it gets worse! Some effects need to modify *both* the input and the output of the function, making them near impossible to implement with a single node in a data-oriented workflow).

[Show fractal noise and the higher-order function approach: passing a noise function to the fractal noise node + this allows creative uses: people passing photos in there and having a nice repetition effect] 

## Implementation details

To pass functions around, we do it when generating the shader code from the nodes: each nodes creates a function, and then those function names are injected in place of the INPUT names. For example, our Disk node might create a `Disk` function, and then, in the code for the grid node, we will replace `Image`, which was the name given to the input function, with `Disk`.
[TODO example of the code before and after name injection]

## Conclusion

You can download it and try it for yourself: https://coollab-art.com/