## Fixing artifacts

Some nodes might not be perfect euclidian distance fields. In such cases visual artifacts can appear:
![Visual artifacts due to approximate distance fields](./with_artifacts.png)

To fix them, simply wrap your return statement.s with `IS0_FIX_ARTIFACTS()`:
```glsl
return IS0_FIX_ARTIFACTS(${SDF}(q)); 
```

It will add a `FixArtifacts` parameter to your nodes.

**N.B.** Always remember that `FixArtifacts` slows down the rendering when set to too high values. Users should always try to keep it as small as possible, and only increase it when they need to fix some visual artifacts.

![Fixed visual artifacts](./without_artifacts.png)