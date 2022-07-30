#pragma once

#include <Cool/Camera/Camera.h>
#include <Cool/Camera/ViewController_Orbital.h>
#include <Cool/Camera/ViewController_OrbitalU.h>
#include <Cool/Dependencies/SetVariableDirty_Ref.h>
#include <Cool/Dependencies/VariableId.h>
#include <Cool/Dependencies/VariableRegistries.h>
#include <Cool/Input/MouseCoordinates.h>
#include <Cool/Input/MouveEventDispatcher.h>
#include <reg/reg.hpp>

namespace Lab {

class CommandExecutor_TopLevel_Ref;
class CommandExecutionContext_Ref;

class CameraManager {
public:
    explicit CameraManager(const Cool::VariableId<Cool::Camera>& camera_id)
        : _camera_id{camera_id}
    {
    }

    CameraManager() = default;

    void hook_events(
        Cool::MouveEventDispatcher<Cool::ViewCoordinates>&,
        std::reference_wrapper<Cool::VariableRegistries>,
        CommandExecutor_TopLevel_Ref
    );

    auto id() const -> const Cool::VariableId<Cool::Camera>& { return _camera_id; }

    void imgui(
        std::reference_wrapper<Cool::VariableRegistries>,
        CommandExecutor_TopLevel_Ref
    );

    auto get_zoom() const -> float { return _view_controller.get_distance_to_orbit_center(); }
    void set_zoom(float zoom, CommandExecutionContext_Ref& ctx);

private:
    void maybe_update_camera(
        std::reference_wrapper<Cool::VariableRegistries>,
        CommandExecutor_TopLevel_Ref,
        std::function<bool(Cool::Camera&)>
    );

private:
    Cool::VariableId<Cool::Camera> _camera_id;
    Cool::ViewController_Orbital   _view_controller;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Camera ID", _camera_id),
            cereal::make_nvp("ViewController", _view_controller)
        );
    }
};

} // namespace Lab