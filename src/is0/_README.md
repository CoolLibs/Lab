# *is0*

## What is *is0* ?

*is0* is a 3D modeler and renderer based on *Signed Distance Fields* (SDFs for short). It is an alternative representation of 3D objects that doesn't involve meshes and is very well suited for procedural modeling and raycast rendering.

## Learning SDFs and RayMarching

I would suggest you watch these videos, in that order: 

- [Conceptual intro to ray marching by *CodeParade*](https://www.youtube.com/watch?v=svLzmFuSBhk)

- [Conceptual intro to ray marching by *Sebastian Lague*](https://www.youtube.com/watch?v=Cp5WWtMoeKg)

- [Practical intro to shader programming](https://www.youtube.com/watch?v=u5HAYVHsasc)

- [Practical intro to ray marching](https://www.youtube.com/watch?v=PGtv-dBi2wE)

## Resources

- [Inigo's Artworks](https://iquilezles.org/www/articles/raymarchingdf/raymarchingdf.htm)
- [List of primitives and effects](https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm)
- [Inigo's painting process](https://youtu.be/8--5LwHRhjk)
- [*Dream*'s engine : different rendering techniques for SDFs](https://www.youtube.com/watch?v=u9KNtnCZDMI)
- [Fuse, a similar project](https://cdm.link/2021/09/fuse-for-vvvv-is-a-visual-revolution-free-always-runtime-patching-right-on-the-gpu/?fbclid=IwAR2nzVaNZO-zYzhiKdFagsm8KpsXMsFCvJwbydii13ILH6RMZb6b0LNJExM)
- [Node Editor in SHADERed](https://shadered.org/plugin?id=spearnode)
- [Performance optimizations for Ray Marching](https://youtu.be/ARlbxXxB1UQ)

## Ideas

```MAX_DIST``` in a ray marcher should be the far plane of the camera (aka length of the image of a canonical ray through the inverse of the projection matrix)

[This article](https://iquilezles.org/www/articles/fbmsdf/fbmsdf.htm) by Inigo would be a great technique to implement.