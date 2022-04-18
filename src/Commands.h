#pragma once
#include <reg/reg.hpp>

namespace Lab {

template<typename T>
struct ReversibleCommand_SetValue {
    reg::Id<T> id;
    T          value;
    T          old_value;
};

using ReversibleCommand = std::variant<
    // ReversibleCommand_SetValue<int>,
    // ReversibleCommand_SetValue<float>,
    // ReversibleCommand_SetValue<glm::vec2>,
    ReversibleCommand_SetValue<glm::vec3> >;

} // namespace Lab
