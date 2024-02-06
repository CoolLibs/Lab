#pragma once
#include "Dependencies/Camera3DManager.h"

namespace Lab {

class MakeReversibleCommandContext_Ref {
public:
    auto camera_manager() const -> const Camera3DManager& { return _data.camera_3D_manager; }

    struct Data { // We wrap our members in a struct to get a constructor automatically
        std::reference_wrapper<Camera3DManager> camera_3D_manager;
    };
    explicit MakeReversibleCommandContext_Ref(Data data)
        : _data{data} {}

private:
    Data _data;
};

} // namespace Lab
