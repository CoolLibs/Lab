# ---HOW TO---

# ------------

import os
from pathlib import Path
from importlib.machinery import SourceFileLoader

generate_files = SourceFileLoader(
    "generate_files",
    os.path.join(
        Path(os.path.abspath(__file__)).parent.parent.parent,
        "Cool",
        "src",
        "Cool",
        "DebugOptions",
        "debug_options_generator.py",
    ),
).load_module()


def all_debug_options():
    from generate_files import DebugOption, Kind

    return [
        DebugOption(
            name_in_code="generate_dump_file",
            name_in_ui="Info Dump: Generate file",
            available_in_release=True,
            kind=Kind.BUTTON,
            detailed_description="Creates an info_dump.txt file next to your executable. It can be used when submitting a bug report, in order to give the devs more information.",
        ),
        DebugOption(
            name_in_code="copy_info_dump_to_clipboard",
            name_in_ui="Info Dump: Copy to clipboard",
            available_in_release=True,
            kind=Kind.BUTTON,
            detailed_description="Copies an info dump to your clipboard. It can be used when submitting a bug report, in order to give the devs more information.",
        ),
        DebugOption(
            name_in_code="show_framerate_window",
            name_in_ui="Framerate window",
            available_in_release=True,
            window_name="Framerate",
            kind=Kind.WINDOW,
            default_value=False,
        ),
        DebugOption(
            name_in_code="show_imgui_demo_window",
            name_in_ui="ImGui Demo window",
            available_in_release=True,
        ),
        DebugOption(
            name_in_code="show_history_window",
            name_in_ui="Show history",
            window_name="History",
            kind=Kind.WINDOW,
            available_in_release=True,
        ),
        DebugOption(
            name_in_code="show_nodes_and_links_registries",
            name_in_ui="Show nodes and links registries",
            available_in_release=True,
        ),
        DebugOption(
            name_in_code="force_rerender_every_frame",
            name_in_ui="Force rerender every frame",
            available_in_release=True,
        ),
        DebugOption(
            name_in_code="log_when_rendering",
            name_in_ui="Log when rendering",
            available_in_release=True,
        ),
        DebugOption(
            name_in_code="log_when_updating_particles",
            name_in_ui="Log when updating particles",
            available_in_release=True,
        ),
        DebugOption(
            name_in_code="log_when_compiling_nodes",
            name_in_ui="Log when compiling nodes",
            available_in_release=True,
        ),
        DebugOption(
            name_in_code="log_when_parsing_node_definition",
            name_in_ui="Log when parsing node definition",
            available_in_release=True,
        ),
        DebugOption(
            name_in_code="log_when_executing_a_command",
            name_in_ui="Log when executing a command",
            available_in_release=True,
        ),
        DebugOption(
            name_in_code="log_project_related_events",
            name_in_ui="Log project-related events",
            available_in_release=True,
        ),
        DebugOption(
            name_in_code="show_generated_shader_code",
            name_in_ui="Show generated shader code",
            available_in_release=True,
            window_name="Nodes Code",
            kind=Kind.WINDOW,
        ),
        DebugOption(
            name_in_code="test_all_variable_widgets__window",
            name_in_ui="Test all Variable Widgets",
            kind=Kind.WINDOW,
            available_in_release=True,
        ),
        DebugOption(
            name_in_code="test_shaders_compilation__window",
            name_in_ui="Test Shaders Compilation",
            kind=Kind.WINDOW,
            available_in_release=True,
        ),
        DebugOption(
            name_in_code="empty_window",
            name_in_ui="Open Empty Window",
            kind=Kind.WINDOW,
            available_in_release=True,
            detailed_description="Useful when you want some blank space in your windows layout.",
        ),
    ]


if __name__ == "__main__":
    from generate_files import generate_debug_options

    generate_debug_options(
        output_folder="generated",
        namespace="Lab",
        cache_file_name="debug-options-lab",
        debug_options=all_debug_options(),
    )
