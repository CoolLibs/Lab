#pragma once

namespace Lab {

struct ProjectManager {
    std::optional<std::filesystem::path> current_project_path{};

    // private:
    //     // Serialization
    //     friend class cereal::access;
    //     template<class Archive>
    //     void serialize(Archive& archive)
    //     {
    //         archive(
    //             cereal::make_nvp("Project path", current_project_path)
    //         );
    //     }
};

} // namespace Lab