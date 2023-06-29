#include "Camera2DManager.h"
#include <Cool/Camera/ViewController_OrbitalU.h>
#include <glm/gtx/matrix_transform_2d.hpp>
#include "Cool/Dependencies/Input.h"
#include "Cool/Image/AspectRatio.h"
#include "Cool/Input/MouseCoordinates.h"
#include "Cool/Input/MouseDragEvents.h"
#include "Cool/StrongTypes/Camera2D.h"
#include "Cool/UserSettings/UserSettings.h"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/string_cast.hpp"
#include "imgui.h"

namespace Lab {

void hook_camera2D_events(
    Cool::MouseEventDispatcher<Cool::ViewCoordinates>& events,
    Cool::Camera2D&                                    camera,
    std::function<void()>                              on_change,
    std::function<bool()>                              is_locked_in_view
)
{
    events
        .scroll_event()
        .subscribe([&, on_change, is_locked_in_view](Cool::MouseScrollEvent<Cool::ViewCoordinates> const& event) {
            if (is_locked_in_view())
                return;

            float const sensitivity    = Cool::user_settings().camera2D_zoom_sensitivity;
            float const zoom_variation = std::pow(sensitivity, event.dy);

            if (!ImGui::GetIO().KeyAlt) // Use ALT to zoom relative to the center of the view
            {
                auto const mouse_pos_in_view_space  = event.position;
                auto const mouse_pos_in_world_space = glm::vec2{camera.transform_matrix() * glm::vec3{mouse_pos_in_view_space, 1.f}};
                auto const rotated_mouse_in_ws      = glm::rotate(mouse_pos_in_world_space, -camera.rotation.as_radians());

                camera.translation = camera.translation / zoom_variation + rotated_mouse_in_ws * (1.f - 1.f / zoom_variation);
            }
            camera.zoom *= zoom_variation;

            on_change();
        });

    events
        .drag()
        .subscribe({
            .on_start  = [is_locked_in_view](auto&&) { return !is_locked_in_view(); },
            .on_update = [&, on_change, is_locked_in_view](Cool::MouseDragUpdateEvent<Cool::ViewCoordinates> const& event) {
                camera.translation -= event.delta / camera.zoom;
                on_change(); },
            .on_stop   = [&](auto&&) {},
        });

    // TODO rotation when shift+scroll and explanation
}

} // namespace Lab