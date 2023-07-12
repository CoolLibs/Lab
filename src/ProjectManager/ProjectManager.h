#pragma once
#include "Project.h"

namespace Lab {

class ProjectManager {
public:
    /// Opens a file dialog and then loads the selected project. Does nothing if the file dialog is cancelled.
    void load(Project&);
    /// Opens a file dialog and then saves the project in the selected file. Does nothing if the file dialog is cancelled.
    void save_as(Project const&);

    void load(Project&, std::filesystem::path const&);
    void save_as(Project const&, std::filesystem::path const&);

    void save(Project const&);

    void imgui(Project&);

    [[nodiscard]] auto current_path() const -> std::filesystem::path const& { return _project_path; }

private:
    std::filesystem::path _project_path{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Project path", _project_path)
        );
    }
};

} // namespace Lab