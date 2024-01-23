#include "Project.h"

namespace Lab {

Project::Project()
    : camera_manager{variable_registries.of<Cool::Variable<Cool::Camera>>().create_shared({})}
    , modules_graph{std::make_unique<ModulesGraph>(dirty_flag_factory(), input_factory())}
{
}

auto Project::is_empty() const -> bool
{
    return modules_graph->is_empty()
           && history.size() == 0;
}

} // namespace Lab