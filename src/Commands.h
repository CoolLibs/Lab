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