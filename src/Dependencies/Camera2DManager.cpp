#include "Camera2DManager.h"
#include <Cool/Camera/ViewController_OrbitalU.h>
#include <glm/gtx/matrix_transform_2d.hpp>
#include "Cool/Dependencies/Input.h"
#include "Cool/Image/AspectRatio.h"
#include "Cool/Input/MouseCoordinates.h"
#include "Cool/Input/MouseDragStartEvent.h"
#include "Cool/StrongTypes/Camera2D.h"
#include "glm/gtx/string_cast.hpp"
#include "imgui.h"

namespace Lab {

void hook_events(
    Cool::MouseEventDispatcher<Cool::ViewCoordinates>& events,
    Cool::Camera2D&                                    camera,
    std::function<void()>                              on_change,
    std::function<float()>                             get_height,
    std::function<float()>                             get_aspect_ratio,
    float&                                             sensitivity
)

{
    events
        // TO DO rotation when shift+scroll and explanation
        .scroll_event()
        .subscribe([&, on_change, get_height, get_aspect_ratio](Cool::MouseScrollEvent<Cool::ViewCoordinates> const& event) {
            float tmp_zoom;
            if (event.dy > 0)
                tmp_zoom = sensitivity;
            else
                tmp_zoom = 1.f / sensitivity;
            auto const mouse_pos_in_view_space  = glm::vec2{event.position / get_height() * 2.f} - glm::vec2(get_aspect_ratio(), 1.f);
            glm::mat3  view_matrix              = camera.transform_matrix();
            auto const mouse_pos_in_world_space = glm::vec2{view_matrix * glm::vec3{mouse_pos_in_view_space, 1.f}};
            float      new_zoom                 = camera.zoom * tmp_zoom;
            camera.translation                  = camera.translation * (1.f / tmp_zoom) + mouse_pos_in_world_space - (1.f / tmp_zoom) * mouse_pos_in_world_space;
            camera.zoom                         = new_zoom;
            on_change();
        });
    events
        .drag()
        .update()
        .subscribe([&, on_change, get_height](Cool::MouseDragUpdateEvent<Cool::ViewCoordinates> const& event) {
            if (event.delta != glm::vec2(0))
            {
                camera.translation -= ((event.delta) / get_height() * 2.f) / camera.zoom;
            }
            on_change();
        });
}

} // namespace Lab