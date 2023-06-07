#pragma once
#include <filesystem>
#include "Cool/StrongTypes/Color.h"
// #include "Cool/Nodes/

namespace Lab {

class NodesCategoryConfig {
public:
    explicit NodesCategoryConfig(std::filesystem::path const& category_folder_path)
        : _path_to_json{category_folder_path / "_category_config.json"}
    {
        load_from_json();
    }

    [[nodiscard]] auto color() const -> Cool::Color { return _color; }
    [[nodiscard]] auto number_of_main_input_pins() const -> size_t { return static_cast<size_t>(_number_of_main_input_pins); }

    auto imgui_popup() -> bool;

private:
    void save_to_json() const;
    void load_from_json();

private:
    Cool::Color           _color{};
    int                   _number_of_main_input_pins{};
    std::filesystem::path _path_to_json{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Color", _color),
            cereal::make_nvp("Number of main input pins", _number_of_main_input_pins)
        );
    }
};

} // namespace Lab