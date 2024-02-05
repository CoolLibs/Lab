#pragma once
#include <Cool/Input/MouseEventDispatcher.h>
#include "Cool/Dependencies/DirtyFlag.h"
#include "Cool/Dependencies/Input.h"
#include "Cool/StrongTypes/Camera2D.h"
#include "Dependencies/Ui.h"

namespace Lab {

class Camera2DManager {
public:
    Camera2DManager() = default;
    Camera2DManager(std::string variable_name, Cool::DirtyFlag const& dirty_flag)
        : _camera{Cool::InputDefinition<Cool::Camera2D>{{std::move(variable_name)}}, dirty_flag}
    {}

    auto               camera() const -> Cool::Camera2D const& { return _camera.value(); }
    auto               camera() -> Cool::Camera2D& { return _camera.value(); }
    [[nodiscard]] auto is_editable_in_view() -> bool& { return _is_editable_in_view; }

    void imgui(Ui_Ref);
    void hook_events(Cool::MouseEventDispatcher<Cool::ViewCoordinates>& events, CommandExecutor const&);

private:
    Cool::Input<Cool::Camera2D> _camera;
    bool                        _is_editable_in_view{true};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Camera", _camera),
            cereal::make_nvp("Is editable in view", _is_editable_in_view)
        );
    }
};

} // namespace Lab