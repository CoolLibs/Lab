# ---HOW TO---

# ------------

def all_debug_options():
    return [
        "show_imgui_debug",
        "show_imgui_demo",
        "show_commands_and_registries_debug_windows",
        "log_when_rendering",
        "test_all_variable_widgets",
    ]


def debug_options_variables():
    return "\n".join(map(lambda debug_option:
                         f"bool {debug_option}{{false}};",
                         all_debug_options()))


def getters_for_debug_build():
    return "\n".join(map(lambda debug_option:
                         f"[[nodiscard]] static auto {debug_option}() -> bool {{ return instance().{debug_option}; }}",
                         all_debug_options()))


def getters_for_release_build():
    return "\n".join(map(lambda debug_option:
                         f"[[nodiscard]] static auto constexpr {debug_option}() -> bool {{ return false; }}",
                         all_debug_options()))


def imgui_checkboxes_for_all_options():
    return "\n".join(map(lambda debug_option:
                         f'ImGui::Checkbox("{debug_option}", &instance().{debug_option});',
                         all_debug_options()))


if __name__ == '__main__':
    from tooling.generate_files import generate
    generate(
        folder="src/Debug/generated",
        files=[
            debug_options_variables,
            getters_for_debug_build,
            getters_for_release_build,
            imgui_checkboxes_for_all_options
        ],
    )
