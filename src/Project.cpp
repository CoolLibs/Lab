#include "Project.h"
#include "Cool/OSC/OSCConnectionEndpoint.h"
#include "Cool/OSC/OSCManager.h"

namespace Lab {

Project::Project()
    : camera_3D_manager{"3D Camera", modules_graph->rerender_all_flag()}
    , camera_2D_manager{"2D Camera", modules_graph->rerender_all_flag()}
{
}

auto Project::is_empty() const -> bool
{
    return modules_graph->is_empty()
           && history.size() == 0
           && Cool::osc_manager().get_connection_endpoint() == Cool::OSCConnectionEndpoint{};
}

} // namespace Lab