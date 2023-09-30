#include "NodesCategoryConfig.h"
#include <Module_Nodes/NodeColor.h>
#include <filesystem>
#include <smart/smart.hpp>
#include "Cool/Path/Path.h"
#include "Cool/Variables/internal/color_utils.h"
#include "Serialization/SNodesCategoryConfig.h"
#include "imgui.h"

namespace Lab {

void NodesCategoryConfig::save_to_json() const
{
    serialize_nodes_cat_cfg(*this, _path_to_json);
}

void NodesCategoryConfig::load_from_json()
{
    deserialize(*this, _path_to_json)
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
        b |= imgui_node_kind_dropdown("Nodes kind", &_nodes_kind);
        b |= ImGui::InputInt("Number of main input pins", &_number_of_main_input_pins);
        _number_of_main_input_pins = smart::keep_above(0, _number_of_main_input_pins);
        ImGui::EndPopup();
    }

    if (b)
        save_to_json();
    return b;
}

} // namespace Lab