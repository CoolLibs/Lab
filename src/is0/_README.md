
# is0

## Learning SDFs and RayMarching

I would suggest you watch all these videos, in that order: 

- [Conceptual intro to ray marching by *CodeParade*](https://www.youtube.com/watch?v=svLzmFuSBhk)

- [Conceptual intro to ray marching by *Sebastian Lague*](https://www.youtube.com/watch?v=Cp5WWtMoeKg)

- [Practical intro to shader programming](https://www.youtube.com/watch?v=u5HAYVHsasc)

- [Practical intro to ray marching](https://www.youtube.com/watch?v=PGtv-dBi2wE)

## Resources

- [*Dream*'s engine : different rendering techniques for SDFs](https://www.youtube.com/watch?v=u9KNtnCZDMI)
- [Fuse, a similar project](https://cdm.link/2021/09/fuse-for-vvvv-is-a-visual-revolution-free-always-runtime-patching-right-on-the-gpu/?fbclid=IwAR2nzVaNZO-zYzhiKdFagsm8KpsXMsFCvJwbydii13ILH6RMZb6b0LNJExM)
- [Node Editor in SHADERed](https://shadered.org/plugin?id=spearnode)

## Ideas

```MAX_DIST``` in a ray marcher should be the far plane of the camera (aka length of the image of a canonical ray through the inverse of the projection matrix)

[This article](https://iquilezles.org/www/articles/fbmsdf/fbmsdf.htm) by Inigo would be a great technique to implement.