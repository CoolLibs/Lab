from dataclasses import dataclass


@dataclass
class PrimitiveType:
    cpp: str
    user_facing_name: str
    corresponding_input_type: str
    glsl: str
    can_be_a_template_type: bool


def all_primitive_types():
    ###
    # NB: The order of definition is important; it corresponds to the order the types will appear in in the dropdown of template node
    ###
    return [
        PrimitiveType(
            cpp="Int",
            user_facing_name="Int",
            corresponding_input_type="int",
            glsl="int",
            can_be_a_template_type=True,
        ),
        PrimitiveType(
            cpp="Float",
            user_facing_name="Float",
            corresponding_input_type="float",
            glsl="float",
            can_be_a_template_type=True,
        ),
        PrimitiveType(
            cpp="Vec2",
            user_facing_name="Vec2",
            corresponding_input_type="glm::vec2",
            glsl="vec2",
            can_be_a_template_type=True,
        ),
        PrimitiveType(
            cpp="Vec3",
            user_facing_name="Vec3",
            corresponding_input_type="glm::vec3",
            glsl="vec3",
            can_be_a_template_type=True,
        ),
        PrimitiveType(
            cpp="Vec4",
            user_facing_name="Vec4",
            corresponding_input_type="glm::vec4",
            glsl="vec4",
            can_be_a_template_type=True,
        ),
        PrimitiveType(
            cpp="RGB",
            user_facing_name="RGB",
            corresponding_input_type="Cool::RgbColor",
            glsl="vec3",
            can_be_a_template_type=False,
        ),
        PrimitiveType(
            cpp="RGBA",
            user_facing_name="RGBA",
            corresponding_input_type="Cool::PremultipliedRgbaColor",
            glsl="vec4",
            can_be_a_template_type=False,
        ),
        PrimitiveType(
            cpp="UV",
            user_facing_name="UV",
            corresponding_input_type="Cool::Point2D",
            glsl="vec2",
            can_be_a_template_type=False,
        ),
        PrimitiveType(
            cpp="SignedDistance",
            user_facing_name="Signed Distance",
            corresponding_input_type=None,
            glsl="float",
            can_be_a_template_type=False,
        ),
        PrimitiveType(
            cpp="Void",
            user_facing_name="Void",
            corresponding_input_type=None,
            glsl="void",
            can_be_a_template_type=False,
        ),
        PrimitiveType(
            cpp="Any",
            user_facing_name="Any",
            corresponding_input_type=None,
            glsl="ERROR the Any type should have been converted earlier in the compilation process.",
            can_be_a_template_type=False,
        ),
    ]


def primitive_types_enum_members():
    from pipe import map
    return ",\n".join(all_primitive_types()
                      | map(lambda type: f"{type.cpp}"))


def glsl_type_as_string_cases():
    from pipe import map
    return "\n".join(all_primitive_types()
                     | map(lambda type: f'case PrimitiveType::{type.cpp}: return "/*{type.cpp}*/ {type.glsl}";'))


def raw_glsl_type_as_string_cases():
    from pipe import map
    return "\n".join(all_primitive_types()
                     | map(lambda type: f'case PrimitiveType::{type.cpp}: return "{type.glsl}";'))


def cpp_type_as_string_cases():
    from pipe import map
    return "\n".join(all_primitive_types()
                     | map(lambda type: f'case PrimitiveType::{type.cpp}: return "{type.cpp}";'))


def check_that_each_primitive_type_corresponds_to_a_different_input_type():
    import collections
    from pipe import where, map

    input_types = list(all_primitive_types()
                       | map(lambda type: type.corresponding_input_type)
                       | where(lambda x: x is not None))

    duplicates = [item for item, count in collections.Counter(input_types).items()
                  if count > 1]

    if len(duplicates) > 0:
        raise Exception(
            f"You can't have two primitive types corresponding to the same input type ({', '.join(duplicates)})")


def input_to_primitive_type():
    from pipe import map
    check_that_each_primitive_type_corresponds_to_a_different_input_type()
    return "\n".join(all_primitive_types()
                     | map(lambda type: f'''
if (std::holds_alternative<Cool::Input<{type.corresponding_input_type}>>(input))
    return PrimitiveType::{type.cpp};''' if type.corresponding_input_type is not None else ""))


def all_template_primitive_types():
    from pipe import where
    return all_primitive_types() | where(lambda type: type.can_be_a_template_type)


def template_node_type_dropdown_string():
    from pipe import where, map

    types_names = (all_template_primitive_types()
                   | map(lambda type: f' {type.user_facing_name}')
                   )

    return '"' + '\\0'.join(types_names) + '\\0"'


def index_generator():
    i = 0
    while True:
        yield i
        i += 1


def type_to_template_combo_index():
    from pipe import where, map
    index = index_generator()
    return "\n".join(all_template_primitive_types()
                     | map(lambda type: f'''case PrimitiveType::{type.cpp}: return {next(index)};''')
                     )


def template_combo_index_to_type():
    from pipe import where, map
    index = index_generator()
    return "\n".join(all_template_primitive_types()
                     | map(lambda type: f'''case {next(index)}: return PrimitiveType::{type.cpp};''')
                     )


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
            raw_glsl_type_as_string_cases,
            cpp_type_as_string_cases,
            input_to_primitive_type,
            template_node_type_dropdown_string,
            type_to_template_combo_index,
            template_combo_index_to_type,
        ],
    )
