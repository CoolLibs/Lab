from tooling.apply_clang_format import apply_clang_format

apply_clang_format("src")
apply_clang_format("res/shader-examples")
apply_clang_format("res/post-processing-examples")
apply_clang_format("res/shader-demo")
apply_clang_format("res/shader-lib")
apply_clang_format("res/is0 shaders", ignored=["hg_sdf.glsl"])
