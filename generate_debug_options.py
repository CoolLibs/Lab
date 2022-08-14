# ---HOW TO---

# ------------

from Cool.src.Cool.DebugOptions.debug_options_generator import DebugOption, generate_debug_options, Kind


def all_debug_options():
    return [
        DebugOption(
            name_in_code="show_framerate_window",
            name_in_ui="Framerate window",
            window_name="Framerate",
            kind=Kind.WINDOW,
            default_value=True,
        ),
        DebugOption(
            name_in_code="show_imgui_demo_window",
            name_in_ui="ImGui Demo window",
        ),
        DebugOption(
            name_in_code="show_commands_and_registries_debug_windows",
            name_in_ui="Commands and Registries windows",
        ),
        DebugOption(
            name_in_code="log_when_rendering",
            name_in_ui="Log when rendering",
        ),
        DebugOption(
            name_in_code="test_all_variable_widgets__window",
            name_in_ui="Test all Variable Widgets",
            kind=Kind.WINDOW,
        ),
        DebugOption(
            name_in_code="test_is0_nodes",
            name_in_ui="Test is0 Nodes",
            kind=Kind.BUTTON,
        ),
    ]


if __name__ == '__main__':
    generate_debug_options(
        output_folder="src/Debug/generated",
        namespace="Lab",
        cache_file_name="cache--debug-options-lab",
        debug_options=all_debug_options(),
    )
