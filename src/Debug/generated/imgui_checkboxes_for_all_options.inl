/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generate_debug_options.py" and edit the "imgui_checkboxes_for_all_options" function there.
 * -----------------------------------------------------------------------------
 */
if (instance().filter.PassFilter("Framerate window")) ImGui::Checkbox("Framerate window", &instance().show_framerate_window);
if (instance().filter.PassFilter("ImGui Demo window")) ImGui::Checkbox("ImGui Demo window", &instance().show_imgui_demo_window);
if (instance().filter.PassFilter("Commands and Registries windows")) ImGui::Checkbox("Commands and Registries windows", &instance().show_commands_and_registries_debug_windows);
if (instance().filter.PassFilter("Log when rendering")) ImGui::Checkbox("Log when rendering", &instance().log_when_rendering);
if (instance().filter.PassFilter("Test all Variable Widgets")) ImGui::Checkbox("Test all Variable Widgets", &instance().test_all_variable_widgets);
if (instance().filter.PassFilter("Test Message Console")) ImGui::Checkbox("Test Message Console", &instance().test_message_console);