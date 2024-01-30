#include "Project.h"

namespace Lab {

Project::Project()
    : camera_manager{Cool::Input<Cool::Camera>{
        Cool::InputDefinition<Cool::Camera>{.name = "3D Camera"},
        modules_graph->rerender_all_flag(),
    }}
{
}

auto Project::is_empty() const -> bool
{
    return modules_graph->is_empty()
           && history.size() == 0;
}

} // namespace Lab