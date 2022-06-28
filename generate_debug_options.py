# ---HOW TO---

# ------------

from dataclasses import dataclass


@dataclass
class DebugOption:
    name_in_code: str
    name_in_ui: str
    default_value: bool = False


def all_debug_options():
    return [
        DebugOption(
            name_in_code="show_framerate_window",
            name_in_ui="Framerate window",
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
            name_in_code="test_all_variable_widgets",
            name_in_ui="Test all Variable Widgets",
        ),
    ]


def debug_options_variables():
    return "\n".join(map(lambda debug_option:
                         f"bool {debug_option.name_in_code}{{{'true' if debug_option.default_value else 'false'}}};",
                         all_debug_options()))


def getters_for_debug_build():
    return "\n".join(map(lambda debug_option:
                         f"[[nodiscard]] static auto {debug_option.name_in_code}() -> bool& {{ return instance().{debug_option.name_in_code}; }}",
                         all_debug_options()))


def getters_for_release_build():
    return "\n".join(map(lambda debug_option:
                         f"[[nodiscard]] static auto constexpr {debug_option.name_in_code}() -> bool {{ return false; }}",
                         all_debug_options()))


def imgui_checkboxes_for_all_options():
    return "\n".join(map(lambda debug_option:
                         f'if (instance().filter.PassFilter("{debug_option.name_in_ui}")) ImGui::Checkbox("{debug_option.name_in_ui}", &instance().{debug_option.name_in_code});',
                         all_debug_options()))


def cereal_make_nvp():
    return ",\n".join(map(lambda debug_option:
                          f'cereal::make_nvp("{debug_option.name_in_ui}", {debug_option.name_in_code})',
                          all_debug_options()))


def reset_all():
    return "\n".join(map(lambda debug_option:
                         f'instance().{debug_option.name_in_code} = {"true" if debug_option.default_value else "false"};',
                         all_debug_options()))


if __name__ == '__main__':
    from tooling.generate_files import generate
    generate(
        folder="src/Debug/generated",
        files=[
            debug_options_variables,
            getters_for_debug_build,
            getters_for_release_build,
            imgui_checkboxes_for_all_options,
            cereal_make_nvp,
            reset_all,
        ],
    )
