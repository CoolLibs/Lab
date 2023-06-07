#include "NodesCategoryConfig.h"
#include <cereal/archives/json.hpp>
#include <filesystem>
#include <smart/smart.hpp>
#include "Cool/Path/Path.h"
#include "Cool/Serialization/Serialization.h"
#include "Cool/Variables/internal/color_utils.h"
#include "imgui.h"

namespace Lab {

void NodesCategoryConfig::save_to_json() const
{
    Cool::Serialization::save<decltype(*this), cereal::JSONOutputArchive>(*this, _path_to_json);
}

void NodesCategoryConfig::load_from_json()
{
    Cool::Serialization::load<decltype(*this), cereal::JSONInputArchive>(*this, _path_to_json)
        .send_error_if_any([&](std::string const& message) {
            return Cool::Message{
                .category = "Nodes Category",
                .message  = fmt::format("Failed to load category config for '{}':\n{}", _path_to_json, message),
                .severity = Cool::MessageSeverity::Warning,
            };
        },
                           Cool::Log::ToUser::console());
}

auto NodesCategoryConfig::imgui_popup() -> bool
{
    bool b = false;

    if (ImGui::BeginPopupContextItem())
    {
        b |= imgui_widget("Color", _color, Cool::internal::color_imgui_flags(false));
        b |= ImGui::InputInt("Number of main input pins", &_number_of_main_input_pins);
        _number_of_main_input_pins = smart::keep_above(0, _number_of_main_input_pins);
        ImGui::EndPopup();
    }

    if (b)
        save_to_json();
    return b;
}

} // namespace Lab