
You have access to quite a few constants and functions like `PI`, `TAU`, `saturate()` and `smooth_min_polynomial()`. For a complete list check out [*Cool/res/shaders/math.glsl*](https://github.com/CoolLibs/Cool/blob/main/res/shaders/math.glsl) and [*res/shader-utils.glsl*](https://github.com/CoolLibs/Lab/tree/main/res/shader-utils.glsl) that are located next to your CoolLab executable.

You can also create your own helper files and `#include` them in your nodes. This can be useful if you want to reuse a big chunk of code in several nodes. To do so, simply create your file in the CoolLab folder and then in your node:
```glsl
#include "_ROOT_FOLDER_/myfile.glsl"
```

NB: \_ROOT\_FOLDER\_ is a special keyword that refers to the CoolLab folder (e.g. the folder containing the CoolLab executable).