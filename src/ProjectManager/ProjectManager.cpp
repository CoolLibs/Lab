#include "ProjectManager.h"
#include <CommandLineArgs/CommandLineArgs.h>
#include <Project.h>
#include <ProjectManager/ProjectManager.h>
#include "Cool/File/File.h"
#include "Cool/Serialization/Serialization.h"
#include "cereal/archives/json.hpp"

namespace Lab {

void ProjectManager::initial_project_loading(Project& project)
{
    // Load the project that was requested, e.g. when double-clicking on a .clb file.
    if (!command_line_args().get().empty())
    {
        load(project, command_line_args().get()[0]);
    }
    // Load the project that was open during the previous session.
    else // TODO(Project) if(project path is valid)
    {
        load(project, _project_path);
    }
    // Fallback to the default project.
    // else {
    //     load(default project);
    // }
}

void ProjectManager::load(Project& project)
{
    auto const path = Cool::File::file_opening_dialog({.file_filters = {{"Coollab project", "clb"}}, .initial_folder = ""}); // TODO(Project) initial_folder should be the folder of _project_path, unless the latter is the path to the default coollab project. In which case leave initial_folder empty.        if (path)
    if (path)
        load(project, *path);
}

void ProjectManager::save_as(Project const& project)
{
    auto const path = Cool::File::file_saving_dialog({.file_filters = {{"Coollab project", "clb"}}, .initial_folder = ""}); // TODO(Project) initial_folder should be the folder of _project_path, unless the latter is the path to the default coollab project. In which case leave initial_folder empty.        if (path)
    if (path)
        save_as(project, *path);
}

void ProjectManager::load(Project& project, std::filesystem::path const& path)
{
    auto const error = Cool::Serialization::load<Project, cereal::JSONInputArchive>(project, path);
    if (error)
    {
        error.send_error_if_any(
            [&](std::string const& message) {
                return Cool::Message{
                    .category = "Loading Project",
                    .message  = message,
                    .severity = Cool::MessageSeverity::Warning,
                };
            },
            Cool::Log::ToUser::console()
        );
        _project_path = "";
        // TODO(Project) Load a default project, otherwise we could probably have corrupted state if only part of the project was loaded.
        return;
    }
    project.is_first_frame = true;
    _project_path = path; // Only assign path if loading was successful.
}

void ProjectManager::save_as(Project const& project, std::filesystem::path const& path)
{
    // TODO(Project) If the path already exists, message box to confirm that user wants to overwrite the project.
    _project_path = path;
    save(project);
}

void ProjectManager::save(Project const& project)
{
    Cool::Serialization::save<Project, cereal::JSONOutputArchive>(project, _project_path, "Project");
}

void ProjectManager::imgui(Project& project)
{
    if (ImGui::MenuItem("Open", "Ctrl+O"))
        load(project);
    if (ImGui::MenuItem("Save As", "Ctrl+S"))
        save_as(project);
}

} // namespace Lab