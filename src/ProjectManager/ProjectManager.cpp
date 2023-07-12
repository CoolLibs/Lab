#include "ProjectManager.h"
#include <Project.h>
#include "Cool/File/File.h"
#include "Cool/Serialization/Serialization.h"
#include "cereal/archives/json.hpp"

namespace Lab {

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
        return;
    }
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
    if (ImGui::Button("Save As"))
    {
        auto const path = Cool::File::file_saving_dialog({.file_filters = {{"Coollab project", "clb"}}, .initial_folder = ""}); // TODO(Project) initial_folder should be the folder of _project_path, unless the latter is the path to the default coollab project. In which case leave initial_folder empty.        if (path)
        if (path)
            save_as(project, *path);
    }
}

} // namespace Lab