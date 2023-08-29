import YoutubeVideo from "@site/src/components/YoutubeVideo"

<YoutubeVideo id="FK1sMPgcE4Q "/>
<br/>

## Summary
In this tutorial, we will have an overview of the different main sections that make up the interface of Coollab.

## Exploring the interface

After opening Coollab, this  pop-up window will appear <span class="icon-bubble"></span> `Did you know?`. We can close it by clicking the `Got it!` button or by clicking the cross in the upper right corner. If we wish to retrieve all of the tips, such as shortcuts, we can click on the appropriate button. We may find the tips in the menu bar, in the commands area, symbolized by the <span class="icon-rocket"></span> icon.

![Pop up](img/Tuto-Interface/PopUp.png)

![Menu bar](img/Tuto-Interface/MenuBar.png)

Nice progress! Now we have the main interface in front of us.

![Coollab Interface](img/Tuto-Interface/Coollab-Interface.png)

Coollab's interface is broken up into several main sections:

- `Nodes panel` <span class="icon-tree"></span> : A place to add your nodes and link them together.
- `Inspector` <span class="icon-equalizer"></span> : Displays information on the selected node.
- `Camera` <span class="icon-camera"></span> : Displays information on the camera and its parameters.
- `View` <span class="icon-image"></span> : The render view of your scene.
- `Time` <span class="icon-stopwatch"></span> : Displays the time in seconds.

Because there is a dock system, we can rearrange them whenever we want. We can simply move the window wherever we wish, if we have another display such as a second monitor, we can drag them there as well. If we only want a floating window, we can temporarily disable docking by pressing `Shift`.

![Docking](img/Tuto-Interface/Docking.png)

## Add nodes in the graph

Let’s jump right inside the **node** graph panel. It is at that location that we will be adding all the nodes and eventually have our project structure. As you can see, we can scroll in and out with our mouse to adjust the zoom inside this window. When we right-click then drag our mouse all over, we can easily navigate through the node graph in order to have a better view or to reorganize our nodes blocks. If we are lost, we can press **F** and it will automatically recenter the view on the nodes.

Now, let’s add our first node to show how the panel works. To add a node, we simply right-click inside the node workspace, then we will see a colored list, corresponding to a specific type of node. Let’s start simple by adding a simple shape. Note that we can also press **A** on our keyboard to display the node menu.

![Node menu](img/Tuto-Interface/NodeDropdownMenu.png)

## Analyzing the node structure

As you can see, a node is generally represented as follows and generally works on their own but there are exceptions such as `Modifier` and `Blend`, we will talk about them later. It consists of one, severals or even none input. The output is always represented with this triangular shape pointing to the right with the word <b>OUT</b>.

![Disk node](img/Tuto-Interface/DiskNode.png)

The `Main node` status actually tells us which node is displayed on the viewport. Let’s show you this feature, if for example we add another node, we can switch between them which can be useful if we wanna see the different steps when we would have a more complex node graph.

![Active node](img/Tuto-Interface/ActiveNode.png)

Now, the idea is to combine and link nodes together. We select the node we want, then we click either on an input or output and drag it out of the node to create a link. We can then connect it to another node. However, if we release it in a blank area, it will display the node menu again. We add one and it will get attached automatically to the other node.

![Link Node](img/Tuto-Interface/LinkNode.gif)

If we want to break the link, we just select the rope and press the **Delete** key on our keyboard.

## End of the tutorial

That’s pretty much all for today’s tutorial! You can already go a little further and try to add nodes by yourself in order to get more familiar with the node panel! 👋



