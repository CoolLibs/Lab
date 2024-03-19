
## 2D and 3D

Particles can be either 2D or 3D:

<div style={{display: "flex", justifyContent: "space-between", gap: "1rem"}}>
<div style={{width: "100%"}}>

```glsl
struct Particle2D {
    vec2  position;
    vec2  velocity;
    vec2  acceleration;
    float size;
    float lifetime;
    float lifetime_max;
    vec4  color;
    uint  index;
    bool  needs_init;
};
```

</div>
<div style={{width: "100%"}}>

```glsl
struct Particle3D {
    vec3  position;
    vec3  velocity;
    vec3  acceleration;
    float size;
    float lifetime;
    float lifetime_max;
    vec4  color;
    uint  index;
    bool  needs_init;
};
```

</div>
</div>

You will notice that the two are very similar, and the effects will be too. This is why you might want to write the effects only once, and have them apply to both 2D and 3D particles.

To do so, simply go to the *Nodes/Particle Nodes Generator* folder and add a *.clbtemp* file in the appropriate folder (*Initializer*, *Force*, etc.). Then run the *particle_nodes_generator.py* script to generate your 2D and 3D files.

```glsl title="Gravity.clbtemp"
INPUT float 'Strength';
INPUT $Direction 'Direction';

$Particle main($Particle Particle)
{
    $vec force = 'Direction' * 'Strength';
    Particle.acceleration += force;
    return Particle;
}
```

*.clbtemp* files are exactly the same as *.clbnode* files, except that you have access to some dollar variables that will be replaced by either their 2D or 3D version. For example `$vec` will become `vec2` for 2D particles and `vec3` for 3D particles.

Here is the list of all of them:

| Template     | 2D            | 3D           |
|--------------|---------------|--------------|
| `$vec`       | `vec2`        | `vec3`       |
| `$vec34`     | `vec3`        | `vec4`       |
| `$UV`        | `UV`          | `vec3`       |
| `$Point`     | `Point2D`     | `vec3`       |
| `$Direction` | `Direction2D` | `vec3`       |
| `$Particle`  | `Particle2D`  | `Particle3D` |

Also, `$vec` is quite smart, so if you write for example `$vec(a, b)` it will become `vec2(a, b)` in 2D and `vec3(a, b, 0.)` in 3D.

You can also write code that will only appear in the 2D or 3D version, just like with an `#ifdef` block:

```glsl
$$if(2D);
vec2 point = random_point_on_disk(random);
$$else;
vec3 point = random_point_on_sphere(random);
$$endif;
```

Finally, `$gradient(p, 'Shape')` will compute the gradient of an SDF. You can use it like this:

```glsl title="Attractor.clbtemp"
INPUT $UV->SignedDistance 'Shape';
INPUT float 'Force';

$Particle main($Particle Particle)
{
    Particle.acceleration += 'Force' * $gradient(Particle.position, 'Shape');
    return Particle;
}
```

Under the hood it will generate a function like this:
```glsl
vec2 gradient(vec2 p)
{
    float h = 0.001;
    return vec2(
               'Shape'(p - vec2(h, 0)) - 'Shape'(p + vec2(h, 0)),
               'Shape'(p - vec2(0, h)) - 'Shape'(p + vec2(0, h))
           )
           / (2. * h);
}
```
and call it with the first parameter `p`.

## Forces

Forces will typically want to add something to the acceleration:

```glsl title="Gravity.clbtemp"
INPUT float 'Strength';
INPUT $Direction 'Direction';

$Particle main($Particle Particle)
{
    $vec force = 'Direction' * 'Strength';
    Particle.acceleration += force;
    return Particle;
}
```

## Initializers

Initializers are responsible for setting one of the attributes of the particle during the first frame of its lifetime. They need to check `Particle.needs_init` to only initialize when necessary:

```glsl title="Init Size.clbtemp"
INPUT float 'Size';

$Particle main($Particle Particle)
{
    if (!Particle.needs_init)
        return Particle;

    Particle.size = 'Size';
    return Particle;
}
```