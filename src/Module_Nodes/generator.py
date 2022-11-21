from dataclasses import dataclass


@dataclass
class PrimitiveType:
    cpp: str
    glsl: str


def all_primitive_types():
    return [
        PrimitiveType(cpp="Color", glsl="vec3"),
        PrimitiveType(cpp="UV", glsl="vec2"),
        # PrimitiveType(cpp="SignedDistance", glsl="float"),
        PrimitiveType(cpp="Float", glsl="float"),
        # PrimitiveType(cpp="Vec3", glsl="vec3"),
        # Trick: because we always want to have exactly one type, we represent Void as an int that will be ignored
        PrimitiveType(cpp="Void", glsl="int"),
    ]


def primitive_types_enum_members():
    return ",\n".join(map(lambda type: f"{type.cpp}", all_primitive_types()))


def glsl_type_as_string_cases():
    return "\n".join(map(lambda type:
                         f'case PrimitiveType::{type.cpp}: return "/*{type.cpp}*/ {type.glsl}";',
                         all_primitive_types()))


def cpp_type_as_string_cases():
    return "\n".join(map(lambda type:
                         f'case PrimitiveType::{type.cpp}: return "{type.cpp}";',
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
            primitive_types_enum_members,
            glsl_type_as_string_cases,
            cpp_type_as_string_cases,
        ],
    )
