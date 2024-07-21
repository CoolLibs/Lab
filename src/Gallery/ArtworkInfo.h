#pragma once

namespace Lab {

struct ArtworkInfo {
    std::string title{};
    std::string description{};

    auto imgui() -> bool;

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Title", title),
            ser20::make_nvp("Description", description)
        );
    }
};

} // namespace Lab