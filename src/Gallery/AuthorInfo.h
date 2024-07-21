#pragma once

namespace Lab {

struct AuthorInfo {
    std::string name{};
    std::string link{};

    auto imgui() -> bool;

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Name", name),
            ser20::make_nvp("Link", link)
        );
    }
};

} // namespace Lab