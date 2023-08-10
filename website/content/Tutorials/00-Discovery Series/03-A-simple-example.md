# A simple example

## Summary

In this tutorial, we are going to dive into the software in order to create a drawing using a couple of nodes.
We will achieve this together, step by step.

We will progress as follows: 

- Presentation of the concept
- Step by step creation with Coollab
- And finally we will export the artwork

Let’s get started!

## Presentation of the concept

As you can see on these images, several artists used flowers or were inspired by them.
We can notice symmetry and repeating patterns.

![FabioCatapano](img/Tuto-A-simple-example/FabioCatapano.jpg)         ![HumbleNovice](img/Tuto-A-simple-example/ParuRaj.jpg)         ![HumbleNovice](img/Tuto-A-simple-example/HumbleNovice.jpg)<br/>

**<i>From left to right: Fabio Catapano - Paru Raj - Humble-Novice</i>**<br/>

Thus, we are going to create a flower-inspired artwork, and this with only five or six nodes!<br/>
Without further ado, let’s get into it!

## Step by step creation with Coollab

First, we will add a `Disk` as the base shape for our flower. We can keep the default parameters. We will then apply several transformations to this disk in order to turn it into a flower.

![Disk Node](img/Tuto-A-simple-example/DiskNode.png)

In order to have a repeating pattern, we will now add a `Grid` node and link it to the previous `Disk` node. We now have a grid made of disks.

![Grid Node](img/Tuto-A-simple-example/GridNode.png)

Next, we are adding a `Mirror Repeat in Circle` node. By clicking and dragging our mouse on the `Period` slider, we are able to modify the value. But if we want to be more accurate, we can also double-click on the slider and input the value of our choice. For our case, let’s put a negative value around -1.350.<br/>
At this time, we have something which looks like a firework star! 🎆

![Mirror Repeat Circle](img/Tuto-A-simple-example/MirrorRepeatCircle.png)

Now’s the time to build the petals! We can add a great `Kaleidoscope` node and… magic! We now have a flower-like shape in the center of the drawing! We can add more petals by increasing the number of `Splits`. We will stick with **six**.

![Kaleidoscope](img/Tuto-A-simple-example/Kaleidoscope.png)

If you accidentally moved inside the view panel, you can reset the position of the camera to the origin by clicking on the <span class="icon-target"></span> button in the bottom right corner of the panel.

![Camera Reset](img/Tuto-A-simple-example/CameraReset.png)

We can zoom in a bit in the view panel using the wheel of the mouse. Before scrolling in, we press the `ALT` key in order to zoom with respect to the center of the view.<br/>
This way, we can adjust the render to keep only the flower and get rid of what’s surrounding it.

![Centered Frame](img/Tuto-A-simple-example/CenteredFrame.png)

In the next step, we will add a bit of glow to the image. To do so, let’s add a `Glow` node and adjust its `Strength` value to 0.008, a reasonable amount. We can find this field inside the Inspector panel when we have the `Glow` node selected.

![Glow Node](img/Tuto-A-simple-example/GlowNode.png)

We have a slight amount of glow on the outlines of the flower.

![Glowing Frame](img/Tuto-A-simple-example/GlowingFrame.png)

Black and white is pretty, but let’s color our flower. We will add a `Color Ramp`.

![Color Ramp](img/Tuto-A-simple-example/ColorRamp.png)

To change the color, we can click on any marker in the `Color Gradient` bar, then on the colored square next to the `-` and `+` button. If we double-click on the marker, we will achieve the same result. We have access to a color panel with a wheel, and a slider for the alpha channel. We can also directly input and modify the color values. Also, it is good to know that if you are happy with the configuration, you can give it a name and save it as a preset by clicking on `Save Preset`.

![Presets](img/Tuto-A-simple-example/Presets.png)

We have an interesting result with the default Candy preset! 😄

![Candy Preset](img/Tuto-A-simple-example/CandyPreset.png)

## Export the artwork

When we are happy with the result from the view panel, we can go over to the <span class="icon-upload2"></span> export section in the menu bar. We will find a dropdown menu. Let’s click on the `Image` option.

![Export Menu](img/Tuto-A-simple-example/ExportMenu.png)

Now, we can choose the resolution and the aspect ratio of the image as well as the name and the path where we want to save our artwork.

![Export Window](img/Tuto-A-simple-example/ExportWindow.png)

:::info Did you know?
Note that you can go to &nbsp;<span class="icon-image"></span> **View > Fixed aspect ratio** in the menu bar to change the aspect ratio and see how the image will look like, according to the chosen ratio, in the view panel.
:::

And well done! We have a beautiful flower!

![Flower](img/Tuto-A-simple-example/Flower.png)

Here are some other examples we made using the same restriction on the number of nodes.

![Noisy Disco](img/Tuto-A-simple-example/NoisyDisco.png)

![Spiraly](img/Tuto-A-simple-example/Spiraly.png)

![Cherry Flake](img/Tuto-A-simple-example/CherryFlake.png)

This the end of this tutorial. It is now your turn to experiment and create your first artwork! Try to keep it simple at first using no more than a couple of nodes 😉. 

Also, feel free to modify the example we have done together, play with different parameters and/or try to add new nodes to the existing graph.

See you in another tutorial of the Coollab discovery series! 👋