// TODO(LD) Move to Cool/res/shaders


# Guide to use Post processing effects

You can create lots of parameters in your shader to modify them in real time. Furthermore, you can add default values to customize your parameters.

*Take a look at what kinds of variable types you can use to do that and how to write their default values:*

- int: `// default 3 min 1 max 5`
- float: `// default 3. min 0.5 max 4.5`
- bool: `// default false`, `// default true`
- vec2 / vec3 / vec4: `// default (3., 2., 5.)`
- RgbColor: *same as vec3*
- Angle: *same as float*
- Direction2D: *same as float* (It is constructed from an angle expressed in turns: `// default 0.25` correspond to a unit vector2D pointing up.)
- Hue: *same as float*. Goes from 0 to 1; 0 and 1 correspond to red.


## Additional rules: 

- `3`, `3.`, `3.f` are all valid syntaxes for all types (even integers).
- The `default` keyword is optional. `Input int my_int; // 3` will create an int with a default value of 3. 
- Each metadata (`default`, `min`, `max`, *etc.*) is optional and will get a default value if not specified.
- Comas are optional