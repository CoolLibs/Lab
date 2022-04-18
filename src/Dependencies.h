// #pragma once

// #include <Cool/Camera/Camera.h>
// #include "Registries.h"
// #include "Time.hpp"

// namespace Lab {

// struct aCamera {
// };

// struct aTimepoint {
// };

// using Dependency = std::variant<aCamera,
//                                 aTimepoint>;

// class Dependencies {
// public:
//     template<typename DependencyT> // DependencyT should be one of the types in the Dependency variant
//     [[nodiscard]] auto get(DependencyT dependency, Lab::Registries& registries) -> DependencyT
//     {
//         const auto it = _map.find(dependency);
//         if (it != _map.end()) {
//             return it.second;
//         }

//         if constexpr (std::is_same_v<T, aCamera>) {
//         }
//         else {
//             throw 0;
//         }
//     }

//     [[nodiscard]] auto contains(reg::AnyId id) const -> bool
//     {
//         for (const auto& [_, someId] : _map) {
//             if (someId == id) {
//                 return true;
//             }
//         }
//         return false;
//     }

// private:
//     std::vector<std::pair<Dependency, reg::AnyId>> _map;
// };

// } // namespace Lab
