#pragma once

#include <Cool/Dependencies/VariableRegistries.h>
#include "Dependencies/CameraManager.h"

namespace Lab {

class MakeReversibleCommandContext_Ref {
public:
    auto registries() const -> const Cool::VariableRegistries& { return _data.registries; }
    auto camera_manager() const -> const CameraManager& { return _data.camera_manager; }

    struct Data { // We wrap our members in a struct to get a constructor automatically
        std::reference_wrapper<Cool::VariableRegistries> registries;
        std::reference_wrapper<CameraManager>            camera_manager;
    };
    explicit MakeReversibleCommandContext_Ref(Data data)
        : _data{data} {}

private:
    Data _data;
};

} // namespace Lab
