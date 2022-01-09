# *is0*

## What is *is0* ?

*is0* is a 3D modeler and renderer based on *Signed Distance Fields* (SDFs for short). It is an alternative representation of 3D objects that doesn't involve meshes and is very well suited for procedural modeling and raycast rendering.

## Learning about SDFs and Ray Marching

You can watch these videos, in that order: 

- [Conceptual intro to ray marching by *CodeParade*](https://www.youtube.com/watch?v=svLzmFuSBhk)

- [Conceptual intro to ray marching by *Sebastian Lague*](https://www.youtube.com/watch?v=Cp5WWtMoeKg)

- [Practical intro to shader programming by *The Art of Code*](https://www.youtube.com/watch?v=u5HAYVHsasc)

- [Practical intro to ray marching by *The Art of Code*](https://www.youtube.com/watch?v=PGtv-dBi2wE)

- [*Inigo Quilez* detailing the creation of one of his artworks](https://youtu.be/-pdSjBPH3zM) (long but very insightful)

- [More videos from *The Art of Code*](https://youtu.be/AfKGMUDWfuE?list=PLGmrMu-IwbgtMxMiV3x4IrHPlPmg7FD-P)

- [More videos from *Inigo Quilez*](https://youtu.be/PMltMdi1Wzg?list=PL0EpikNmjs2CYUMePMGh3IjjP4tQlYqji)

## Resources about SDFs

- [List of primitives and effects](https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm)

- [Inigo's Artworks](https://iquilezles.org/www/articles/raymarchingdf/raymarchingdf.htm)

- [Inigo's painting process](https://youtu.be/8--5LwHRhjk)

- [Rendering SDFs as smoke](https://wallisc.github.io/rendering/2020/05/02/Volumetric-Rendering-Part-2.html)

- [*Dream*'s engine: different rendering techniques for SDFs](https://www.youtube.com/watch?v=u9KNtnCZDMI)

- [Non-euclidian space rendered with Ray Marching and SDFs](https://youtu.be/ivHG4AOkhYA)

- [Performance optimizations for Ray Marching](https://youtu.be/ARlbxXxB1UQ)

- [Another list full of resources](https://github.com/CedricGuillemet/SDF)

## Other SDF-based tools

- [MagicaCSG](https://youtu.be/Z1qj9xgqdps)

- [Unity Raymarching Toolkit](https://youtu.be/QDfqgG8HJDQ)

- [RayTK for TouchDesigner](https://t3kt.github.io/raytk/)

## Other node-based tools

- [Unity's Shadergraph](https://youtu.be/Ar9eIn4z6XE)

- [Blender's material nodes](https://youtu.be/BDtVsS_zR_8)

- [Blender's geometry nodes](https://youtu.be/TjGL4RjR13Q)

- [Node Editor in SHADERed](https://shadered.org/plugin?id=spearnode)

- [Fuse for vvv](https://cdm.link/2021/09/fuse-for-vvvv-is-a-visual-revolution-free-always-runtime-patching-right-on-the-gpu/?fbclid=IwAR2nzVaNZO-zYzhiKdFagsm8KpsXMsFCvJwbydii13ILH6RMZb6b0LNJExM)

## Code structure

### Is0

`Is0` is the top-level class. It owns a `NodeEditor` and compiles the shader from the `NodeTree` owned by the `NodeEditor`.

### NodeTree

`NodeTree` is a simple class that holds a list of `Node`s and a list of `Link`s. It provides utility functions to add, delete and find nodes and links.

It is independent of any UI library and does not deal with displaying the tree or handling user interactions.

### NodeEditor

It is `NodeEditor` that handles the UI side. We use the [ImNodes](https://github.com/Nelarius/imnodes) library to display our nodes and links and handle user interactions to create or delete nodes and links.

`NodeEditor` owns a `NodeTree` and modifies it to reflect the user actions provided by `ImNodes`.

It also owns a `NodeFactory` to create the `Node`s.

### NodeFactory

`NodeFactory` owns the `NodeTemplate`s that we read from the *.is0* files and use to create the different node types.

`NodeFactory` is responsible for loading the `NodeTemplate`s and creating or updating a node from a `NodeTemplate`.

### NodeTemplate

A `NodeTemplate` is the data that allows us to know what a `Node` should be like (how many input pins and which kind of parameters), and holds the shader code for the given node type.

For example if we create a *Sphere* node from the corresponding template, the shader code is held by the `NodeTemplate` since all *Sphere* nodes will share the exact same code.

### Node

A `Node` holds all relevant data to be displayed and to generate some shader code, a.k.a.:
- The name of the `NodeTemplate` so that we can get the shader code (we reference the `NodeTemplate` by name instead of pointer in order to be able to serialize the node and still find the template back).

- The `Cool::Parameter`s of the node, using the `Cool::ParameterDesc`s from the `NodeTemplate`.

## Implementation ideas

### Material system

What is a material? For the moment it will be a simple struct containing some properties of the object: albedo, roughness, metalic, etc. In the future we might try to support a more complex system like Blender's shading nodes which allows per-pixel calculations to affect the material, basically allowing us to texture our objects. For now, we will stick to one fixed material per object.

But now, how do we go from SDFs, that tell us that there is "something" at a given position, to knowing the material of the pixel we are currently shading? Each object could have an ID, and its SDF would return both the distance and the ID. Then the ID would be used to look up the associated material. This is nice and simple **but** how do we handle blending materials? For a smooth min we would like to smoothly blend the materials of the two objects in the transition area. If we want to support blending materials, no ID-based system will ever work, because there will be infinitely many shades of material in-between two given materials.
We therefore need to return materials directly. Doing it in the regular SDF would be a waste of effort because the SDF is called many times, but the materials is only used once at the end. We will therefore creaete another function, sibling of the SDF, that returns the material of the nearest object for a given position in space (instead of the distance to that nearest object).

For primitives, this will simply return the struct with the values provided by the user. For a min it will compute which object is the closest and return its material. For a smooth min it will compute a blending factor based on the distance to the two objects, and will blend the materials accordingly, and so on.
The material function should be defined in the node file alongside the SDF. It can have parameters (one of the type beeing: the material struct, for primitives).

Users should also be able to override materials: for example a smooth min could have one single material struct chosen by the user, instead of blending the materials of its children. Its only if no material is specified for the smooth min that the blending occurs.

UI: it could be a simple input where your select your materia (Blender-like)l. You can create new materials, or share materials across objects. There would be a *default* material for primitives (white) and a *None* option for Booleans and Modifiers, which means that the material(s) of the child(ren) object(s) is(are) used.