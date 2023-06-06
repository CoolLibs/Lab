#pragma once

namespace Lab {

struct AuthorInfo {
    std::string name{};
    std::string link{};

    auto imgui() -> bool;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Name", name),
            cereal::make_nvp("Link", link)
        );
    }
};

} // namespace Lab