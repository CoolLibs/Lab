A `PostProcess` should describe what it needs as input: which color space, which meaning of alpha, if it needs a depth texture, etc. And it should also describe what it outputs: which color space etc. This would allow us to do all the implicit conversions needed.

It can have more than one input image: color, depth, a mask (aka alpha more or less) etc.

Each effect can be controlled by a slider from 0 to 1 (or even outside this range). This allows for example to have a fade off on the edges of mask (half transparent edges).