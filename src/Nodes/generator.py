
from dataclasses import dataclass


@dataclass
class PrimitiveType:
    cpp: str
    glsl: str


def all_primitive_types():
    return [
        PrimitiveType(cpp="RgbColor", glsl="vec3"),
        PrimitiveType(cpp="UV", glsl="vec2"),
        PrimitiveType(cpp="SignedDistance", glsl="float"),
        PrimitiveType(cpp="Float", glsl="float"),
    ]


def primitive_types_list():
    return ",\n".join(map(lambda type: f"PrimitiveType::{type.cpp}", all_primitive_types()))


def primitive_types_structs_definitions():
    return "\n".join(map(lambda type: f"struct {type.cpp} {{}};", all_primitive_types()))


def glsl_type_as_string():
    return "\n".join(map(lambda type:
                         f'if constexpr (std::is_same_v<T, PrimitiveType::{type.cpp}>) return "{type.glsl}";',
                         all_primitive_types()))


def cpp_type_as_string():
    return "\n".join(map(lambda type:
                         f'if constexpr (std::is_same_v<T, PrimitiveType::{type.cpp}>) return "{type.cpp}";',
                         all_primitive_types()))


if __name__ == '__main__':
    # HACK: Python doesn't allow us to import from a parent folder (e.g. tooling.generate_files)
    # So we need to add the path manually to sys.path
    import os
    import sys
    from pathlib import Path
    sys.path.append(os.path.join(
        Path(os.path.abspath(__file__)).parent.parent.parent,
        "tooling")
    )
    # End of HACK
    import generate_files

    generate_files.generate(
        folder="generated",
        files=[
            primitive_types_list,
            primitive_types_structs_definitions,
            glsl_type_as_string,
            cpp_type_as_string,
        ],
    )
