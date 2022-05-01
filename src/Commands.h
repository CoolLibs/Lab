#pragma once
#include <Cool/Camera/Camera.h>
#include <reg/reg.hpp>

namespace Lab {

template<typename T>
struct ReversibleCommand_SetValue {
    reg::Id<T> id;
    T          value;
    T          old_value;
};

template<typename T>
struct Command_SetValue {
    reg::Id<T> id;
    T          value;
};

/// Submit this when for example you finished dragging a slider (e.g. when ImGui::IsItemDeactivatedAfterEdit() is true)
/// It will make sure that the commit will be locked in the history and future commands modifying the same value
/// won't be merged with the ones that were issued before Command_FinishedEditingValue
struct Command_FinishedEditingValue {
};

using Command = std::variant<
    Command_FinishedEditingValue,

    Command_SetValue<int>,
    Command_SetValue<float>,
    Command_SetValue<Cool::Camera>,
    Command_SetValue<glm::vec2>,
    Command_SetValue<glm::vec3>>;

using ReversibleCommand = std::variant<
    ReversibleCommand_SetValue<int>,
    ReversibleCommand_SetValue<float>,
    ReversibleCommand_SetValue<Cool::Camera>,
    ReversibleCommand_SetValue<glm::vec2>,
    ReversibleCommand_SetValue<glm::vec3>>;

} // namespace Lab

#include <cereal/types/variant.hpp>

namespace cereal {

template<class Archive, typename T>
void serialize(Archive& archive, Lab::ReversibleCommand_SetValue<T>& command)
{
    archive(command.id,
            command.value,
            command.old_value);
}

} // namespace cereal