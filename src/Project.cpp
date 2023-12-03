#include "Project.h"

namespace Lab {

Project::Project()
    : camera_manager{variable_registries.of<Cool::Variable<Cool::Camera>>().create_shared({})}
    , nodes_graph{std::make_unique<NodesGraph>(dirty_flag_factory(), input_factory())}
{
}

auto Project::is_empty() const -> bool
{
    return nodes_graph->is_empty()
           && history.size() == 0;
}

} // namespace Lab