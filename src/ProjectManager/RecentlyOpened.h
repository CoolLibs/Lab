#pragma once
#include <filesystem>
#include <optional>

namespace Lab {

class RecentlyOpened {
public:
    void on_project_opened(std::filesystem::path const&);
    auto get_list() -> std::vector<std::filesystem::path> const&;
    auto imgui() -> std::optional<std::filesystem::path>;

private:
    std::vector<std::filesystem::path> _list{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("List", _list)
        );
    }
};

} // namespace Lab