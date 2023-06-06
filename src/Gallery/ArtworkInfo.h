#pragma once

namespace Lab {

struct ArtworkInfo {
    std::string title{};
    std::string description{};

    auto imgui() -> bool;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Title", title),
            cereal::make_nvp("Description", description)
        );
    }
};

} // namespace Lab