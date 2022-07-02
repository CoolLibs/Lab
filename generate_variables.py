# ---HOW TO---
# Create a function with the same name as the file you want to generate
# that returns a string with the content you want to generate.
#
# Then add your function to the call to `generate()` at the end of the file.
#
# You can use `all_variable_types()` to get all the variable types we use in CoolLab.
# ------------

from dataclasses import dataclass


@dataclass
class VariableMetadata:
    name_in_shader: str
    field_name: str


@dataclass
class VariableType:
    type: str
    metadatas: list[VariableMetadata]


def all_variable_types_and_metadata():
    return [
        VariableType(
            type="bool",
            metadatas=[]
        ),
        VariableType(
            type="int",
            metadatas=[
                VariableMetadata(
                    name_in_shader="min",
                    field_name="min_value",
                ),
                VariableMetadata(
                    name_in_shader="max",
                    field_name="max_value",
                ),
            ]
        ),
        VariableType(
            type="float",
            metadatas=[
                VariableMetadata(
                    name_in_shader="min",
                    field_name="min_value",
                ),
                VariableMetadata(
                    name_in_shader="max",
                    field_name="max_value",
                ),
            ]
        ),
        VariableType(
            type="glm::vec2",
            metadatas=[]
        ),
        VariableType(
            type="glm::vec3",
            metadatas=[]
        ),
        VariableType(
            type="glm::vec4",
            metadatas=[]
        ),
        VariableType(
            type="Cool::RgbColor",
            metadatas=[
                VariableMetadata(
                    name_in_shader="hdr",
                    field_name="is_hdr",
                ),
            ]
        ),
        VariableType(
            type="Cool::Camera",
            metadatas=[]
        ),
        VariableType(
            type="Cool::Angle",
            metadatas=[]
        ),
        VariableType(
            type="Cool::Direction2D",
            metadatas=[]
        ),
    ]


def all_variable_types():
    return map(lambda type_and_meta: type_and_meta.type,
               all_variable_types_and_metadata())


def all_variable_includes():
    return f"""
#include <Cool/Camera/Camera.h>
#include <Cool/Variables/Variables.h>
"""


def register_set_variable_commands():
    commands = ""
    reversible_commands = ""
    for variable_type in all_variable_types():
        commands += f"LAB_REGISTER_COMMAND(Lab::Command_SetVariable<{variable_type}>)\n"
        reversible_commands += f"LAB_REGISTER_REVERSIBLE_COMMAND(Lab::ReversibleCommand_SetVariable<{variable_type}>)\n"
    return f"""
{commands}
{reversible_commands}
"""


def register_set_variable_metadata_commands():
    commands = "\n"
    for variable_type in all_variable_types():
        commands += f"LAB_REGISTER_COMMAND(Lab::Command_SetVariableMetadata<{variable_type}>)\n"
    return commands


def VariableRegistries():
    return "\n" + "using VariableRegistries = reg::Registries<\n" + ",\n".join(
        map(lambda var_type: f"    Cool::Variable<{var_type}>", all_variable_types())) + "\n>;"


def AnyInput():
    return "\n" + "using AnyInput = std::variant<\n" + ",\n".join(
        map(lambda var_type: f"    Input<{var_type}>", all_variable_types())) + "\n>;"


def AnyInputRef():
    return "\n" + "using AnyInputRef = std::variant<\n" + ",\n".join(
        map(lambda var_type: f"    std::reference_wrapper<Input<{var_type}>>", all_variable_types())) + "\n>;"


def AnyInputRefToConst():
    return "\n" + "using AnyInputRefToConst = std::variant<\n" + ",\n".join(
        map(lambda var_type: f"    std::reference_wrapper<const Input<{var_type}>>", all_variable_types())) + "\n>;"


if __name__ == '__main__':
    from tooling.generate_files import generate
    generate(
        folder="src/generated_variables",
        files=[
            register_set_variable_commands,
            register_set_variable_metadata_commands,
            VariableRegistries,
            AnyInput,
            AnyInputRef,
            AnyInputRefToConst,
            all_variable_includes,
        ],
    )
