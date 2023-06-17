from dataclasses import dataclass
from typing import Optional, List, Any


@dataclass
class PrimitiveType:
    cpp: str
    user_facing_name: str
    corresponding_input_types: List[str]
    glsl: str
    parsed_from: Optional[str]
    can_be_a_template_type: bool


def all_primitive_types():
    ###
    # NB: The order of definition is important; it corresponds to the order the types will appear in in the dropdown of template node
    ###
    res = [
        PrimitiveType(
            cpp="Bool",
            user_facing_name="Bool",
            corresponding_input_types=["bool"],
            glsl="bool",
            parsed_from="bool",
            can_be_a_template_type=False,
        ),
        PrimitiveType(
            cpp="Int",
            user_facing_name="Int",
            corresponding_input_types=["int"],
            glsl="int",
            parsed_from="int",
            can_be_a_template_type=True,
        ),
        PrimitiveType(
            cpp="Float",
            user_facing_name="Float",
            corresponding_input_types=["float"],
            glsl="float",
            parsed_from="float",
            can_be_a_template_type=True,
        ),
        PrimitiveType(
            cpp="Vec2",
            user_facing_name="2D Vector",
            corresponding_input_types=["glm::vec2"],
            glsl="vec2",
            parsed_from="vec2",
            can_be_a_template_type=True,
        ),
        PrimitiveType(
            cpp="Vec3",
            user_facing_name="3D Vector",
            corresponding_input_types=["glm::vec3"],
            glsl="vec3",
            parsed_from="vec3",
            can_be_a_template_type=True,
        ),
        PrimitiveType(
            cpp="Vec4",
            user_facing_name="4D Vector",
            corresponding_input_types=["glm::vec4"],
            glsl="vec4",
            parsed_from="vec4",
            can_be_a_template_type=True,
        ),
        PrimitiveType(
            cpp="Mat2",
            user_facing_name="2D Matrix",
            corresponding_input_types=["glm::mat2"],
            glsl="mat2",
            parsed_from="mat2",
            can_be_a_template_type=False,
        ),
        PrimitiveType(
            cpp="Mat3",
            user_facing_name="3D Matrix",
            corresponding_input_types=["glm::mat3"],
            glsl="mat3",
            parsed_from="mat3",
            can_be_a_template_type=False,
        ),
        PrimitiveType(
            cpp="Mat4",
            user_facing_name="4D Matrix",
            corresponding_input_types=["glm::mat4"],
            glsl="mat4",
            parsed_from="mat4",
            can_be_a_template_type=False,
        ),
        PrimitiveType(
            cpp="UV",
            user_facing_name="UV",
            corresponding_input_types=["Cool::Point2D"],
            glsl="vec2",
            parsed_from="UV",
            can_be_a_template_type=False,
        ),
        PrimitiveType(
            cpp="SignedDistance",
            user_facing_name="Signed Distance",
            corresponding_input_types=[],
            glsl="float",
            parsed_from="SignedDistance",
            can_be_a_template_type=False,
        ),
        PrimitiveType(
            cpp="Angle",
            user_facing_name="Angle",
            corresponding_input_types=["Cool::Angle"],
            glsl="float",
            parsed_from="Angle",
            can_be_a_template_type=False,
        ),
        PrimitiveType(
            cpp="Hue",
            user_facing_name="Hue",
            corresponding_input_types=["Cool::Hue"],
            glsl="float",
            parsed_from="Hue",
            can_be_a_template_type=False,
        ),
        PrimitiveType(
            cpp="Direction2D",
            user_facing_name="Direction 2D",
            corresponding_input_types=["Cool::Direction2D"],
            glsl="vec2",
            parsed_from="Direction2D",
            can_be_a_template_type=False,
        ),
        PrimitiveType(
            cpp="Void",
            user_facing_name="Void",
            corresponding_input_types=[],
            glsl="void",
            parsed_from=None,
            can_be_a_template_type=False,
        ),
        PrimitiveType(
            cpp="Any",
            user_facing_name="Any",
            corresponding_input_types=[],
            glsl="ERROR the Any type should have been converted earlier in the compilation process.",
            parsed_from="Any",
            can_be_a_template_type=False,
        ),
        PrimitiveType(  # TODO(JF) Remove this once helper functions can use whatever type they want.
            cpp="RayMarchRes",
            user_facing_name="RayMarchRes",
            corresponding_input_types=[],
            glsl="RayMarchRes",
            parsed_from="RayMarchRes",
            can_be_a_template_type=False,
        ),
    ]

    res.extend(primitive_types_for_color_spaces())

    return res


def color_spaces():
    # HACK: Python doesn't allow us to import from a parent folder (e.g. tooling.generate_files)
    # So we need to add the path manually to sys.path
    import os
    import sys
    from pathlib import Path

    sys.path.append(
        os.path.join(
            Path(os.path.abspath(__file__)).parent.parent.parent,
            "Cool/src/Cool/ColorSpaces",
        )
    )
    # End of HACK

    import generator_colors

    return generator_colors.color_spaces()


def primitive_types_for_color_spaces() -> List[PrimitiveType]:
    res = []

    for color_space in color_spaces():
        # No alpha
        res.append(
            PrimitiveType(
                cpp=color_space.name_in_code,
                user_facing_name=color_space.user_facing_name,
                corresponding_input_types=["Cool::Color"],
                glsl="vec3",
                parsed_from=color_space.name_in_code,
                can_be_a_template_type=False,
            )
        )
        # Premultiplied alpha
        res.append(
            PrimitiveType(
                cpp=color_space.name_in_code + "_PremultipliedA",
                user_facing_name=color_space.user_facing_name + ", Premultiplied Alpha",
                corresponding_input_types=["Cool::ColorAndAlpha"],
                glsl="vec4",
                parsed_from=color_space.name_in_code + "_PremultipliedA",
                can_be_a_template_type=False,
            )
        )
        # Straight alpha
        res.append(
            PrimitiveType(
                cpp=color_space.name_in_code + "_StraightA",
                user_facing_name=color_space.user_facing_name + ", Straight Alpha",
                corresponding_input_types=["Cool::ColorAndAlpha"],
                glsl="vec4",
                parsed_from=color_space.name_in_code + "_StraightA",
                can_be_a_template_type=False,
            )
        )

    return res


def alpha_spaces():
    return ["", "_StraightA", "_PremultipliedA"]


def implicit_color_conversions():
    from itertools import product

    res = ""
    for color1, color2 in product(color_spaces(), color_spaces()):
        for alpha1, alpha2 in product(alpha_spaces(), alpha_spaces()):
            type1 = color1.name_in_code + alpha1
            type2 = color2.name_in_code + alpha2
            if type1 == type2:
                continue

            def gen_code(in_vec: str, out_vec: str, implementation: str):
                nonlocal res
                res += f"""
                if (from == PrimitiveType::{type1} && to == PrimitiveType::{type2})
                {{
                    return context.push_function({{
                        .name           = "Coollab_{type2}_from_{type1}",
                        .definition = R"STR(
                            {out_vec} Coollab_{type2}_from_{type1}/*coollabdef*/({in_vec} from)
                            {{
                                {implementation}
                            }}
                            )STR",
                    }});
                }}
                """

            color_conversion = (
                f"Cool_{color2.name_in_code}_from_{color1.name_in_code}"
                if color1 != color2
                else ""
            )
            match alpha1, alpha2:
                case "", "":
                    gen_code(
                        in_vec="vec3",
                        out_vec="vec3",
                        implementation=f"""
                        vec3 to = {color_conversion}(from);
                        return to;
                    """,
                    )
                case "_StraightA", "":  # We can afford to lose the alpha information because it is stored in the coollab_context anyways.
                    gen_code(
                        in_vec="vec4",
                        out_vec="vec3",
                        implementation=f"""
                        vec3 to = {color_conversion}(from.xyz);
                        return to;
                    """,
                    )
                case "_PremultipliedA", "":  # We can afford to lose the alpha information because it is stored in the coollab_context anyways.
                    gen_code(
                        in_vec="vec4",
                        out_vec="vec3",
                        implementation=f"""
                        vec3 to = {color_conversion}(from.xyz / saturate(from.a));
                        return to;
                    """,
                    )
                case "", "_StraightA":
                    gen_code(
                        in_vec="vec3",
                        out_vec="vec4",
                        implementation=f"""
                        vec3 to = {color_conversion}(from);
                        return vec4(to, coollab_global_alpha);
                    """,
                    )
                case "", "_PremultipliedA":
                    gen_code(
                        in_vec="vec3",
                        out_vec="vec4",
                        implementation=f"""
                        vec3 to = {color_conversion}(from);
                        return vec4(to, 1.) * coollab_global_alpha;
                    """,
                    )
                case "_StraightA", "_StraightA":
                    gen_code(
                        in_vec="vec4",
                        out_vec="vec4",
                        implementation=f"""
                        vec3 to = {color_conversion}(from.xyz);
                        return vec4(to, from.a);
                    """,
                    )
                case "_PremultipliedA", "_PremultipliedA":
                    gen_code(
                        in_vec="vec4",
                        out_vec="vec4",
                        implementation=f"""
                        // We need to unpremultiply for the color conversion, and re-premultiply afterwards
                        vec3 to = {color_conversion}(from.xyz / saturate(from.a));
                        return vec4(to * saturate(from.a), from.a);
                    """,
                    )
                case "_PremultipliedA", "_StraightA":
                    gen_code(
                        in_vec="vec4",
                        out_vec="vec4",
                        implementation=f"""
                        vec3 to = {color_conversion}(from.xyz / saturate(from.a));
                        return vec4(to, from.a);
                    """,
                    )
                case "_StraightA", "_PremultipliedA":
                    gen_code(
                        in_vec="vec4",
                        out_vec="vec4",
                        implementation=f"""
                        vec3 to = {color_conversion}(from.xyz);
                        return vec4(to * saturate(from.a), from.a);
                    """,
                    )
    return res


def has_an_alpha_channel():
    res = ""
    for color_space in color_spaces():
        res += f"case PrimitiveType::{color_space.name_in_code}_StraightA:\n"
        res += f"case PrimitiveType::{color_space.name_in_code}_PremultipliedA:\n"

    res += "return true;"
    return res


def is_color_type():
    from itertools import product

    def code(color_space, alpha_space):
        return f"type == PrimitiveType::{color_space.name_in_code}{alpha_space}"

    return "\n ||".join(
        map(lambda x: code(x[0], x[1]), product(color_spaces(), alpha_spaces()))
    )


def primitive_types_enum_members():
    from pipe import map

    return ",\n".join(all_primitive_types() | map(lambda type: f"{type.cpp}"))


def glsl_type_as_string_cases():
    from pipe import map

    return "\n".join(
        all_primitive_types()
        | map(
            lambda type: f'case PrimitiveType::{type.cpp}: return "/*{type.cpp}*/ {type.glsl}";'
        )
    )


def raw_glsl_type_as_string_cases():
    from pipe import map

    return "\n".join(
        all_primitive_types()
        | map(lambda type: f'case PrimitiveType::{type.cpp}: return "{type.glsl}";')
    )


def cpp_type_as_string_cases():
    from pipe import map

    return "\n".join(
        all_primitive_types()
        | map(lambda type: f'case PrimitiveType::{type.cpp}: return "{type.cpp}";')
    )


def check_that_each_primitive_type_corresponds_to_a_different_input_type():
    import collections
    from pipe import chain, map

    input_types = list(
        all_primitive_types() | map(lambda type: type.corresponding_input_types) | chain
    )

    duplicates = [
        item
        for item, count in collections.Counter(input_types).items()
        if count > 1 and item != "Cool::Color" and item != "Cool::ColorAndAlpha"
    ]

    if len(duplicates) > 0:
        raise Exception(
            f"You can't have two primitive types corresponding to the same input type ({', '.join(duplicates)})"
        )


def check_color_space(type):
    if type[0] == "Cool::Color":
        return f"&& std::get<Cool::Input<{type[0]}>>(input)._desired_color_space == static_cast<int>(Cool::ColorSpace::{type[1]})"
    elif type[0] == "Cool::ColorAndAlpha":
        return f"&& std::get<Cool::Input<{type[0]}>>(input)._desired_color_space == static_cast<int>(Cool::ColorAndAlphaSpace::{type[1]})"
    else:
        return ""


def input_to_primitive_type():
    from pipe import map, chain

    check_that_each_primitive_type_corresponds_to_a_different_input_type()
    return "\n".join(
        all_primitive_types()
        | map(
            lambda type: type.corresponding_input_types | map(lambda t: [t, type.cpp])
        )
        | chain
        | map(
            lambda type: f"""
if (std::holds_alternative<Cool::Input<{type[0]}>>(input) {check_color_space(type)})
    return PrimitiveType::{type[1]};"""
        )
    )


def all_template_primitive_types():
    from pipe import where

    return all_primitive_types() | where(lambda type: type.can_be_a_template_type)


def template_node_type_dropdown_string():
    from pipe import where, map

    types_names = all_template_primitive_types() | map(
        lambda type: f" {type.user_facing_name}"
    )

    return '"' + "\\0".join(types_names) + '\\0"'


def type_to_template_combo_index():
    from pipe import map
    import itertools

    index = itertools.count()
    return "\n".join(
        all_template_primitive_types()
        | map(lambda type: f"""case PrimitiveType::{type.cpp}: return {next(index)};""")
    )


def template_combo_index_to_type():
    from pipe import map
    import itertools

    index = itertools.count()
    return "\n".join(
        all_template_primitive_types()
        | map(lambda type: f"""case {next(index)}: return PrimitiveType::{type.cpp};""")
    )


def parse_primitive_type():
    from pipe import map, where

    return "\n".join(
        all_primitive_types()
        | where(lambda x: x.parsed_from is not None)
        | map(
            lambda type: f"""if (str == "{type.parsed_from}") {{ return PrimitiveType::{type.cpp}; }}"""
        )
    )


def string_listing_the_parsed_types():
    from pipe import map, where

    return (
        'R"STR('
        + "\n".join(
            all_primitive_types()
            | where(lambda x: x.parsed_from is not None)
            | map(lambda type: f"  - {type.parsed_from}")
        )
        + ')STR"'
    )


if __name__ == "__main__":
    # HACK: Python doesn't allow us to import from a parent folder (e.g. tooling.generate_files)
    # So we need to add the path manually to sys.path
    import os
    import sys
    from pathlib import Path

    sys.path.append(
        os.path.join(Path(os.path.abspath(__file__)).parent.parent.parent, "tooling")
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
            parse_primitive_type,
            string_listing_the_parsed_types,
            implicit_color_conversions,
            has_an_alpha_channel,
            is_color_type,
        ],
    )
