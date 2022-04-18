#pragma once

#include <Cool/Camera/Camera.h>
#include <reg/reg.hpp>

/// All the registries are owned by the top-level app, in order to simplify the changes through commands, the history, etc.

namespace Lab {

struct Registries {
    reg::Registry<Cool::Camera> cameras{};
    reg::Registry<float>        floats{};
    reg::Registry<glm::vec3>    vec3s{};

    template<typename T>
    auto get() -> reg::Registry<T>&
    {
        if constexpr (std::is_same_v<T, Cool::Camera>) {
            return cameras;
        }
        if constexpr (std::is_same_v<T, float>) {
            return floats;
        }
        if constexpr (std::is_same_v<T, glm::vec3>) {
            return vec3s;
        }
    }
};

} // namespace Lab
