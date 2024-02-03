#include "Camera2DManager.h"
#include <Cool/Camera/ViewController_OrbitalU.h>
#include <glm/gtx/matrix_transform_2d.hpp>
#include "Cool/Dependencies/Input.h"
#include "Cool/Input/MouseCoordinates.h"
#include "Cool/Input/MouseDragEvents.h"
#include "Cool/StrongTypes/Camera2D.h"
#include "Cool/UserSettings/UserSettings.h"
#include "glm/gtx/rotate_vector.hpp"
#include "imgui.h"

namespace Lab {

void Camera2DManager::hook_events(Cool::MouseEventDispatcher<Cool::ViewCoordinates>& events)
{
    events
        .scroll_event()
        .subscribe([&](Cool::MouseScrollEvent<Cool::ViewCoordinates> const& event) {
            if (!_is_editable_in_view)
                return;

            float const sensitivity    = Cool::user_settings().camera2D_zoom_sensitivity;
            float const zoom_variation = std::pow(sensitivity, event.dy);

            if (!ImGui::GetIO().KeyAlt) // Use ALT to zoom relative to the center of the view
            {
                auto const mouse_pos_in_view_space  = event.position;
                auto const mouse_pos_in_world_space = glm::vec2{_camera.value().transform_matrix() * glm::vec3{mouse_pos_in_view_space, 1.f}};
                auto const rotated_mouse_in_ws      = glm::rotate(mouse_pos_in_world_space, -_camera.value().rotation.as_radians());

                _camera.value().translation = _camera.value().translation / zoom_variation + rotated_mouse_in_ws * (1.f - 1.f / zoom_variation);
            }
            _camera.value().zoom *= zoom_variation;

            _camera.dirty_flag().set_dirty();
        });

    events
        .drag()
        .subscribe({
            .on_start  = [&](auto&&) { return _is_editable_in_view; },
            .on_update = [&](Cool::MouseDragUpdateEvent<Cool::ViewCoordinates> const& event) {
                _camera.value().translation -= event.delta / _camera.value().zoom;
                _camera.dirty_flag().set_dirty();
                ImGui::WrapMousePos(ImGuiAxesMask_All);
                //
            },
            .on_stop = [&](auto&&) {},
        });

    // TODO(Camera2D) rotation when shift+scroll and explanation
}

void Camera2DManager::imgui()
{
    if (imgui_widget(_camera.variable())) // TODO(Variables) Just use the usual imgui() for Inputs (from the Ui class ?)
        _camera.dirty_flag().set_dirty();
}

} // namespace Lab