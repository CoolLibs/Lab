#pragma once

#include <optional>
namespace Lab {

struct LegalInfo {
    std::string email{};
    bool        has_agreed{false};

    void               imgui();
    [[nodiscard]] auto missing_information_message() const -> std::optional<std::string>;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Email", email)
        );
    }
};

} // namespace Lab