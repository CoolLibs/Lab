
# Guide to use Post processing effects

  

You can create lots of parameters in your shader to modify them in real time. Furthermore, you can add default values to personalyze your parameters.

*So take a look on what kinds of variable types you can use to do that and how to write their default values :*

- int 
*-- ex :* ```// default 3 (3.f or 3.) min 1 max 5```
- float
*-- ex :* ```// default 3.f (or 3. or 3) min 0.5 max 4.5```
- bool
*-- ex :* ```//default false (or true)```
- vec2 / vec3 / vec4
*-- ex :* ```//default vec(3., 2., 5.) (or 3 2 5)```
- RgbColor
*-- ex :* ```//default vec(3., 2., 5.) (or 3. 2. 5.)```
- Angle
*-- same as float*
- Direction2D
*-- same as float*
- Hue
*-- same as float*


## Additional rules : 

- 3 = 3. = 3.f for each types, it will be converted.
- Default is optional
- You can only set default min or max without specify others
- Coma is optional
- Number of spaces is not important