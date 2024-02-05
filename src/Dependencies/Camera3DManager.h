#pragma once
#include <Cool/Camera/Camera.h>
#include <Cool/Camera/ViewController_Orbital.h>
#include <Cool/Camera/ViewController_OrbitalU.h>
#include <Cool/Dependencies/Input.h>
#include <Cool/Input/MouseCoordinates.h>
#include <Cool/Input/MouseEventDispatcher.h>

namespace Lab {

class CommandExecutionContext_Ref;
class CommandExecutor;

class Camera3DManager {
public:
    Camera3DManager() = default;
    Camera3DManager(std::string variable_name, Cool::DirtyFlag const& dirty_flag)
        : _camera_input{Cool::InputDefinition<Cool::Camera>{{std::move(variable_name)}}, dirty_flag}
    {}

    auto               camera() const -> Cool::Camera const& { return _camera_input.value(); }
    [[nodiscard]] auto is_editable_in_view() -> bool& { return _is_editable_in_view; }

    void hook_events(
        Cool::MouseEventDispatcher<Cool::ViewCoordinates>&,
        CommandExecutor const&
    );

    void imgui(CommandExecutor const&);

    [[nodiscard]] auto get_zoom() const -> float { return _view_controller.get_distance_to_orbit_center(); }
    void               set_zoom(float zoom, CommandExecutionContext_Ref const& ctx);

    void reset_camera(CommandExecutor const&);

private:
    void maybe_update_camera(
        CommandExecutor const&,
        std::function<bool(Cool::Camera&)> const&
    );

private:
    Cool::Input<Cool::Camera>    _camera_input;
    Cool::ViewController_Orbital _view_controller;
    bool                         _is_editable_in_view{true};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Camera", _camera_input),
            cereal::make_nvp("View controller", _view_controller),
            cereal::make_nvp("Is editable in view", _is_editable_in_view)
        );
    }
};

} // namespace Lab