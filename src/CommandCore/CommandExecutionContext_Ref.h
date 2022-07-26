#pragma once

#include <Cool/Dependencies/Dirty.h>
#include <Cool/Dependencies/SetVariableDirty_Ref.h>
#include <Cool/Dependencies/VariableId.h>
#include <Cool/Dependencies/VariableRegistries.h>
#include "Dependencies/History.h"

namespace Lab {

class CameraManager;

// This is a class rather than a struct because we want to use methods to access the different members
// this will make it easier to track down who is using what
// and since this class is a big thing that makes everything accessible to everyone, it is especially important to be able to track the use that is made of each member
class CommandExecutionContext_Ref {
public:
    auto history() const -> const History& { return _data.history; }
    auto registries() -> Cool::VariableRegistries& { return _data.registries; }
    auto camera_manager() -> CameraManager& { return _data.camera_manager; }
    template<typename T>
    void set_dirty(const Cool::VariableId<T>& id)
    {
        _data.set_dirty(id);
    }

    struct Data { // We wrap our members in a struct to get a constructor automatically
        std::reference_wrapper<const History>            history;
        std::reference_wrapper<Cool::VariableRegistries> registries;
        std::reference_wrapper<CameraManager>            camera_manager;
        Cool::SetVariableDirty_Ref                       set_dirty;
    };
    explicit CommandExecutionContext_Ref(Data data)
        : _data{data} {}

private:
    Data _data;
};

} // namespace Lab