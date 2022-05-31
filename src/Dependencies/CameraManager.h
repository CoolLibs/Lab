#pragma once

#include <Cool/Camera/Camera.h>
#include <Cool/Camera/ViewController_Orbital.h>
#include <Cool/Input/MouseCoordinates.h>
#include <Cool/Input/MouveEventDispatcher.h>
#include <reg/reg.hpp>
#include "Dependencies/CommandsExec.h"
#include "Dependencies/VariableRegistries.h"

namespace Lab {

class CommandDispatcher;

class CameraManager {
public:
    explicit CameraManager(const reg::Id<Cool::Camera>& camera_id)
        : _camera_id{camera_id}
    {
    }

    CameraManager() = default;

    void hook_events(Cool::MouveEventDispatcher<Cool::ViewCoordinates>&, std::reference_wrapper<VariableRegistries>, CommandDispatcher);

    auto id() const -> const reg::Id<Cool::Camera>& { return _camera_id; }

    void imgui(std::reference_wrapper<VariableRegistries> registries,
               CommandDispatcher                          commander);

private:
    void maybe_update_camera(std::reference_wrapper<VariableRegistries>, CommandDispatcher, std::function<bool(Cool::Camera&)> fun);

private:
    reg::Id<Cool::Camera>        _camera_id;
    Cool::ViewController_Orbital _view_controller;
    float                        _last_aspect_ratio{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Camera ID", _camera_id),
                cereal::make_nvp("ViewController", _view_controller));
    }
};

} // namespace Lab