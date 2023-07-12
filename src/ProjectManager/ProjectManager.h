#pragma once
#include "Project.h"

namespace Lab {

class ProjectManager {
public:
    void load(Project&, std::filesystem::path const&);
    void save_as(Project const&, std::filesystem::path const&);
    void save(Project const&);

    void imgui(Project&);

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