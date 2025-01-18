#include "ProjectManager.hpp"
#include <ImGuiNotify/ImGuiNotify.hpp>
#include <filesystem>
#include <reg/src/generate_uuid.hpp>
#include "COOLLAB_FILE_EXTENSION.hpp"
#include "Cool/CommandLineArgs/CommandLineArgs.h"
#include "Cool/File/File.h"
#include "Cool/ImGui/ImGuiExtrasStyle.h"
#include "Cool/Log/ToUser.h"
#include "Cool/Path/Path.h"
#include "Debug/DebugOptions.h"
#include "ProjectManagerImpl.hpp"
#include "boxer/boxer.h"

namespace Lab {

// TODO(Launcher) notification to confirm save when pressing CTRL+S (+mention there is autosave(if it is enabled) so there is no need to press CTRL+S)
// TODO(Launcher) handle invalid file names (with invalid chars, with dots . , with / or \)

void ProjectManager::process_command_line_args(OnProjectLoaded const& on_project_loaded, SetWindowTitle const& set_window_title)
{
    for (size_t i = 0; i < Cool::command_line_args().get().size(); ++i)
    {
        auto const& arg = Cool::command_line_args().get()[i];
        Cool::Log::Debug::info("cli", arg);
        if (arg == "--create_new_project_in_folder" || arg == "--create_new_project_in_file")
        {
            if (i + 1 == Cool::command_line_args().get().size())
            {
                create_new_project(on_project_loaded, set_window_title);
                continue;
            }

            auto const& arg2 = Cool::command_line_args().get()[i + 1];
            if (arg2.starts_with("--"))
            {
                create_new_project(on_project_loaded, set_window_title);
                continue;
            }
            if (arg == "--create_new_project_in_folder")
                create_new_project_in_folder(arg2, on_project_loaded, set_window_title);
            else
                create_new_project_in_file(arg2, on_project_loaded, set_window_title);
            continue;
        }
        if (arg == "--open_project")
        {
            if (i + 1 == Cool::command_line_args().get().size())
            {
                // TODO(Launcher) warning
                continue;
            }

            auto const& arg2 = Cool::command_line_args().get()[i + 1];
            if (arg2.starts_with("--"))
            {
                // TODO(Launcher) warning
                continue;
            }

            open_project(arg2, on_project_loaded, set_window_title);
            continue;
        }
        if (arg == "--projects_info_folder_for_the_launcher")
        {
            if (i + 1 == Cool::command_line_args().get().size())
            {
                // TODO(Launcher) warning
                continue;
            }

            auto const& arg2 = Cool::command_line_args().get()[i + 1];
            if (arg2.starts_with("--"))
            {
                // TODO(Launcher) warning
                continue;
            }

            _impl.set_projects_info_folder_for_the_launcher(arg2);
            continue;
        }
    }
    // In case no command line args told us what to do (which should only happen when Coollab is not launched via the launcher, i.e. only while developing Coollab)
    // then always open the same project, so that it is convenient while developing to access your test project immediately
    if (_impl.project_folder().empty())
        open_project(Cool::File::with_extension(Cool::Path::user_data() / "Projects/Untitled", COOLLAB_FILE_EXTENSION), on_project_loaded, set_window_title);
}

void ProjectManager::create_new_project(OnProjectLoaded const& on_project_loaded, SetWindowTitle const& set_window_title)
{
    create_new_project_in_folder(Cool::Path::user_data() / "Projects", on_project_loaded, set_window_title);
}

void ProjectManager::create_new_project_in_folder(std::filesystem::path const& folder_path, OnProjectLoaded const& on_project_loaded, SetWindowTitle const& set_window_title)
{
    create_new_project_in_file(Cool::File::with_extension(folder_path / "Untitled", COOLLAB_FILE_EXTENSION), on_project_loaded, set_window_title);
}

void ProjectManager::create_new_project_in_file(std::filesystem::path file_path, OnProjectLoaded const& on_project_loaded, SetWindowTitle const& set_window_title)
{
    if (DebugOptions::log_project_related_events())
        Cool::Log::ToUser::info("Project", fmt::format("Created new project in \"{}\"", file_path));

    file_path = Cool::File::find_available_path(file_path);

    auto project = Project{};
    project.clock.pause();
    project.camera_3D_manager.is_editable_in_view() = false;

    _impl.set_project(std::move(project), on_project_loaded);
    _impl.set_project_path(file_path, set_window_title);
    // _impl.register_last_write_time(file_path); // No need
}

void ProjectManager::open_project(std::filesystem::path const& file_path, OnProjectLoaded const& on_project_loaded, SetWindowTitle const& set_window_title)
{
    if (DebugOptions::log_project_related_events())
        Cool::Log::ToUser::info("Project", fmt::format("Opening project \"{}\"", Cool::File::weakly_canonical(file_path)));

    auto maybe_project = _impl.load(file_path);
    if (!maybe_project.has_value())
    {
        ImGuiNotify::send({
            .type     = ImGuiNotify::Type::Error,
            .title    = fmt::format("Failed to open project \"{}\"", Cool::File::weakly_canonical(file_path)),
            .content  = maybe_project.error(),
            .duration = std::nullopt,
        });
        create_new_project_in_file(file_path, on_project_loaded, set_window_title);
        return;
    }

    _impl.set_project(std::move(maybe_project.value()), on_project_loaded);
    _impl.set_project_path(file_path, set_window_title);
    _impl.register_last_write_time(file_path);
}

void ProjectManager::autosave_project(SetWindowTitle const& set_window_title)
{
    if (_impl.project().is_empty()) // Avoid creating project files if we are just opening and then closing Coollab
        return;

    if (DebugOptions::log_project_related_events())
        Cool::Log::ToUser::info("Project", fmt::format("Autosaving project \"{}\"", Cool::File::weakly_canonical(_impl.project_path())));

    // TODO(Launcher) if file_contains_data_that_we_did_not_write_ourselves(), dialog to save in another file
    // "This file has been modified externally. We cannot save there because it would overwrite the changes."
    while (!_impl.save(_impl.project_path()))
        _impl.set_project_path(force_file_dialog_to_save_project(), set_window_title); // Save failed, try in another location.
    _impl.register_last_write_time(_impl.project_path());
    _impl.set_project_path_for_launcher(_impl.project_path());
}

void ProjectManager::save_project(SetWindowTitle const& set_window_title)
{
    if (DebugOptions::log_project_related_events())
        Cool::Log::ToUser::info("Project", fmt::format("Saving project \"{}\"", Cool::File::weakly_canonical(_impl.project_path())));

    if (_impl.file_contains_data_that_we_did_not_write_ourselves(_impl.project_path()))
    {
        // We are overwriting a file that we did not write ourselves, this is dangerous because we might overwrite data saved by the user
        boxer::show(
            "This file has been modified externally. We cannot save there because it would overwrite the changes.\nPlease select another location to save the file.",
            "Cannot save here",
            boxer::Style::Warning,
            boxer::Buttons::OK
        );
        auto const file_path = file_dialog_to_save_project();
        if (!file_path.has_value())
            return;
        _impl.set_project_path(*file_path, set_window_title);
    }

    while (!_impl.save(_impl.project_path()))
        _impl.set_project_path(force_file_dialog_to_save_project(), set_window_title); // Save failed, try in another location.

    _impl.register_last_write_time(_impl.project_path());
    _impl.set_project_path_for_launcher(_impl.project_path());

    ImGuiNotify::send({
        .type     = ImGuiNotify::Type::Success,
        .title    = "Saved Project",
        .content  = "Note that there is autosave",
        .duration = 2s,
    });
}

void ProjectManager::save_project_as(std::filesystem::path file_path, SetWindowTitle const& set_window_title)
{
    if (DebugOptions::log_project_related_events())
        Cool::Log::ToUser::info("Project", fmt::format("Saving project as \"{}\"", Cool::File::weakly_canonical(file_path)));

    // TODO(Launcher) setting to choose if we make this path the new path for the project, or if this is just a one-off save
    // By default this should just be a one-off save

    bool const this_is_the_new_project_path = false;

    auto const previous_uuid = _impl.project().uuid;
    _impl.project().uuid     = reg::generate_uuid(); // This new project path should be associated with a new uuid

    while (!_impl.save(file_path))
        file_path = force_file_dialog_to_save_project(); // Save failed, try in another location

    _impl.register_last_write_time(file_path);
    _impl.set_project_path_for_launcher(file_path);

    if (this_is_the_new_project_path)
    {
        _impl.set_project_path(file_path, set_window_title);
    }
    else
    {
        _impl.project().uuid = previous_uuid; // Keep the same uuid as before because we are still on the same project
    }
}

void ProjectManager::package_project_into(std::filesystem::path const& folder_path, SetWindowTitle const& set_window_title)
{
    if (DebugOptions::log_project_related_events())
        Cool::Log::ToUser::info("Project", fmt::format("Packaging project into \"{}\"", Cool::File::weakly_canonical(folder_path)));

    // TODO(Launcher) make sure the folder doesn't exist, or is empty
    auto file_path = folder_path;
    file_path.replace_extension(COOLLAB_FILE_EXTENSION);
    save_project_as(file_path, set_window_title); // TODO(Project) Implement the packaging-specific stuff like copying images and nodes.
}

void ProjectManager::rename_project(std::string new_name, SetWindowTitle const& set_window_title)
{
    if (DebugOptions::log_project_related_events())
        Cool::Log::ToUser::info("Project", fmt::format("Renaming project as \"{}\"", new_name));

    // TODO(Launcher) make sure the name is a valid file name, with no ".", no "/" or "\"
    new_name = Cool::File::find_available_name(_impl.project_folder(), new_name, COOLLAB_FILE_EXTENSION).string();
    // TODO(Launcher)warning if we changed the name of the project because another file with the same name already exists

    if (_impl.file_contains_data_that_we_did_not_write_ourselves(project_path()))
    {
        // Somehow, someone wrote something to this file, so we will not overwrite it, just in case they saved something important
        _impl.set_project_path(_impl.project_path(new_name), set_window_title);
        while (!_impl.save(_impl.project_path()))
            _impl.set_project_path(force_file_dialog_to_save_project(), set_window_title);
        // TODO(Launcher) warning to the user
    }
    else
    {
        // We are the ones who last wrote to this file, so we are allowed to delete it
        Cool::File::rename(_impl.project_path(), _impl.project_path(new_name));
        // TODO(Launcher) tell the launcher that the file has been renamed
    }
    _impl.set_project_name(new_name, set_window_title);
    _impl.register_last_write_time(_impl.project_path());
    _impl.set_project_path_for_launcher(_impl.project_path());
}

void ProjectManager::imgui_project_name_in_the_middle_of_the_menu_bar(SetWindowTitle const& set_window_title)
{
    static auto project_name         = ""s;
    const char* default_project_name = "Untitled Project";

    // TODO(Launcher) make this an ImGuiExtras widget
    auto const width = ImGui::CalcTextSize(project_name.empty() ? default_project_name : project_name.c_str()).x
                       + 2.f * Cool::ImGuiExtras::GetStyle().tab_bar_padding.x;
    ImGui::SetNextItemWidth(width);
    ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.5f - width * 0.5f);

    ImGui::InputTextWithHint("##project_name", default_project_name, &project_name);
}

auto ProjectManager::file_dialog_to_save_project() -> std::optional<std::filesystem::path>
{
    return Cool::File::file_saving_dialog(Cool::File::file_dialog_args{
        .file_filters   = {{"Coollab project", COOLLAB_FILE_EXTENSION}},
        .initial_folder = _impl.project_folder(),
    });
}

auto ProjectManager::force_file_dialog_to_save_project() -> std::filesystem::path
{
    while (true)
    {
        auto const path = file_dialog_to_save_project();
        if (path.has_value())
            return *path;
    }
}

} // namespace Lab