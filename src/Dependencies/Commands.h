#pragma once
#include <Cool/Camera/Camera.h>
#include <reg/reg.hpp>
#include "AllParameterTypes.h"

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

using Command =
    AllParameterTypes::
        wrap<Command_SetValue>::
            append<Command_FinishedEditingValue>::
                to<std::variant>;

using ReversibleCommand =
    AllParameterTypes::
        wrap<ReversibleCommand_SetValue>::
            to<std::variant>;

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