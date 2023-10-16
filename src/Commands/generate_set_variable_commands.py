if (
    True
):  # HACK: Without this "if True", the code gets formatted and "import generate_variables" moves above the hack, which makes it fail
    # HACK: Python doesn't allow us to import from a parent folder (e.g. tooling.generate_files)
    # So we need to add the path manually to sys.path
    import os
    import sys
    from pathlib import Path

    sys.path.append(
        os.path.join(
            Path(os.path.abspath(__file__)).parent.parent.parent,
            "Cool/src/Cool/Variables",
        )
    )
    # End of HACK
    import generator_variables


def register_set_variable_commands():
    commands = ""
    reversible_commands = ""
    for variable_type in generator_variables.all_variable_types():
        commands += f"LAB_REGISTER_COMMAND(Lab::Command_SetVariable<{variable_type}>)\n"
        reversible_commands += f"LAB_REGISTER_REVERSIBLE_COMMAND(Lab::ReversibleCommand_SetVariable<{variable_type}>)\n"
    return f"""
{commands}
{reversible_commands}
"""


def register_set_variable_metadata_commands():
    out = "\n"
    for variable_type in generator_variables.all_variable_types():
        out += f"LAB_REGISTER_COMMAND(Lab::Command_SetMetadata<{variable_type}>)\n"
    return out


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
            register_set_variable_commands,
            register_set_variable_metadata_commands,
        ],
    )
