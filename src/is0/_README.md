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

`NodeEditor` owns a `NodeTree` and modifies it accordingly to reflect the user actions provided by `ImNodes`.

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