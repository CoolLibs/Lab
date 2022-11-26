from dataclasses import dataclass


@dataclass
class PrimitiveType:
    cpp: str
    corresponding_input_type: str
    glsl: str


def all_primitive_types():
    return [
        PrimitiveType(
            cpp="RGB",
            corresponding_input_type="Cool::RgbColor",
            glsl="vec3"
        ),
        PrimitiveType(
            cpp="RGBA",
            corresponding_input_type="Cool::PremultipliedRgbaColor",
            glsl="vec4"
        ),
        PrimitiveType(
            cpp="UV",
            corresponding_input_type="Cool::Point2D",
            glsl="vec2"),
        PrimitiveType(
            cpp="SignedDistance",
            corresponding_input_type=None,
            glsl="float"
        ),
        PrimitiveType(
            cpp="Float",
            corresponding_input_type="float",
            glsl="float"
        ),
        PrimitiveType(
            cpp="Vec3",
            corresponding_input_type="glm::vec3",
            glsl="vec3"
        ),
        PrimitiveType(
            cpp="Void",
            corresponding_input_type=None,
            glsl="void"
        ),
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


def check_that_each_primitive_type_corresponds_to_a_different_input_type():
    input_types = []
    for type in all_primitive_types():
        if type.corresponding_input_type is not None:
            input_types.append(type.corresponding_input_type)

    for i in range(len(input_types)):
        for j in range(i):
            if input_types[i] == input_types[j]:
                raise Exception(
                    f"You can't have two primitive types corresponding to the same input type ({input_types[i]})")


def input_to_primitive_type():
    check_that_each_primitive_type_corresponds_to_a_different_input_type()
    return "\n".join(map(lambda type: f'''
if (std::holds_alternative<Cool::Input<{type.corresponding_input_type}>>(input))
    return PrimitiveType::{type.cpp};''' if type.corresponding_input_type is not None else "",
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
            input_to_primitive_type,
        ],
    )
