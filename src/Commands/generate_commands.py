# ---HOW TO---
# Create a function with the same name as the file you want to generate
# that returns a string with the content you want to generate.
#
# Then add your function to the call to `generate()` at the end of the file.
#
# You can use `all_variable_types()` to get all the variable types we use in Coollab.
# ------------

from dataclasses import dataclass
from collections.abc import Callable


@dataclass
class SetterCommand:
    name: str
    user_facing_name: str
    data_type: str
    get_value: str
    set_value: Callable[[str], str]  # value_name -> function_implementation
    before_command_declaration: str  # template, documenting comment
    extra_data: str  # eg an id to know which object to set


def command_set(cmd: SetterCommand):
    return f"""
#include <stringify/stringify.hpp>
#include "CommandCore/CommandExecutionContext_Ref.h"
#include "CommandCore/MakeReversibleCommandContext_Ref.h"

namespace Lab {{

struct ReversibleCommand_{cmd.name};

struct Command_{cmd.name} {{
    {cmd.data_type} value{{}};

    void execute(CommandExecutionContext_Ref const& ctx) const
    {{
        {cmd.set_value("value")}
    }}

    auto to_string() const -> std::string
    {{
        return "Set {cmd.user_facing_name} to " + Cool::stringify(value);
    }}

    auto make_reversible(const MakeReversibleCommandContext_Ref& ctx) const -> ReversibleCommand_{cmd.name};
}};

struct ReversibleCommand_{cmd.name} {{
    Command_{cmd.name} forward_command{{}};
    {cmd.data_type}    old_value{{}};

    void execute(CommandExecutionContext_Ref const& ctx) const
    {{
        forward_command.execute(ctx);
    }}

    void revert(CommandExecutionContext_Ref const& ctx) const
    {{
        {cmd.set_value("old_value")}
    }}

    auto to_string() const -> std::string
    {{
        return "Set {cmd.user_facing_name} from " + Cool::stringify(old_value) +
               " to " + Cool::stringify(forward_command.value);
    }}

    auto merge(const ReversibleCommand_{cmd.name}& previous) const -> std::optional<ReversibleCommand_{cmd.name}>
    {{
        return ReversibleCommand_{cmd.name}{{
            .forward_command = forward_command,
            .old_value       = previous.old_value,
        }};
    }};
}};

inline auto Command_{cmd.name}::make_reversible(const MakeReversibleCommandContext_Ref& ctx) const
    -> ReversibleCommand_{cmd.name}
{{
    return ReversibleCommand_{cmd.name}{{
        .forward_command = *this,
        .old_value       = {cmd.get_value},
    }};
}}

}} // namespace Lab

namespace cereal {{

template<class Archive>
void serialize(Archive& archive, Lab::Command_{cmd.name}& command)
{{
    archive(cereal::make_nvp("{cmd.user_facing_name}", command.value));
}}

template<class Archive>
void serialize(Archive& archive, Lab::ReversibleCommand_{cmd.name}& command)
{{
    archive(
        cereal::make_nvp("{cmd.user_facing_name}", command.forward_command.value),
        cereal::make_nvp("Old {cmd.user_facing_name}", command.old_value)
    );
}}

}} // namespace cereal

"""


def Command_SetCameraZoom():
    return command_set(
        SetterCommand(
            name="SetCameraZoom",
            user_facing_name="camera zoom",
            data_type="float",
            get_value="ctx.camera_manager().get_zoom()",
            set_value=lambda value: f"ctx.camera_manager().set_zoom({value}, ctx);",
            before_command_declaration="",
            extra_data="",
        )
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
        # TODO(JF) Make both script generate in the same folder
        folder="generated2",
        files=[
            Command_SetCameraZoom,
        ],
    )
