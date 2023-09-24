#pragma once

#include <Cool/Camera/Camera.h>
#include <Cool/Camera/ViewController_Orbital.h>
#include <Cool/Camera/ViewController_OrbitalU.h>
#include <Cool/Dependencies/SetVariableDirty_Ref.h>
#include <Cool/Dependencies/VariableId.h>
#include <Cool/Dependencies/VariableRegistries.h>
#include <Cool/Input/MouseCoordinates.h>
#include <Cool/Input/MouseEventDispatcher.h>
#include <reg/reg.hpp>
#include <utility>

namespace Lab {

class CommandExecutionContext_Ref;
class CommandExecutor;

class CameraManager {
public:
    explicit CameraManager(Cool::SharedVariableId<Cool::Camera> camera_id)
        : _camera_id{std::move(camera_id)}
    {
    }

    CameraManager() = default;

    void hook_events(
        Cool::MouseEventDispatcher<Cool::ViewCoordinates>&,
        std::reference_wrapper<Cool::VariableRegistries>,
        CommandExecutor const&,
        std::function<void()> on_change
    );

    [[nodiscard]] auto id() const -> Cool::SharedVariableId<Cool::Camera> { return _camera_id; }
    [[nodiscard]] auto is_editable_in_view() -> bool& { return _is_editable_in_view; }

    void imgui(
        std::reference_wrapper<Cool::VariableRegistries>,
        CommandExecutor const&,
        std::function<void()> on_change
    );

    [[nodiscard]] auto get_zoom() const -> float { return _view_controller.get_distance_to_orbit_center(); }
    void               set_zoom(float zoom, CommandExecutionContext_Ref const& ctx);

    void reset_camera(
        std::reference_wrapper<Cool::VariableRegistries>,
        CommandExecutor const&,
        std::function<void()> on_change
    );

private:
    void maybe_update_camera(
        std::reference_wrapper<Cool::VariableRegistries>,
        CommandExecutor const&,
        std::function<void()> on_change,
        std::function<bool(Cool::Camera&)>
    );

private:
    Cool::SharedVariableId<Cool::Camera> _camera_id;
    Cool::ViewController_Orbital         _view_controller;
    bool                                 _is_editable_in_view{true};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Camera ID", _camera_id),
            cereal::make_nvp("ViewController", _view_controller),
            cereal::make_nvp("Is editable in view", _is_editable_in_view)
        );
    }
};

} // namespace Lab