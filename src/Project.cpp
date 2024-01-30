#include "Project.h"

namespace Lab {

Project::Project()
    : camera_manager{{}}
{
}

auto Project::is_empty() const -> bool
{
    return modules_graph->is_empty()
           && history.size() == 0;
}

} // namespace Lab