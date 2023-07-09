#include "Project.h"

namespace Lab {

Project::Project()
    : camera_manager{variable_registries.of<Cool::Variable<Cool::Camera>>().create_shared({})}
    , nodes_module{std::make_unique<Module_Nodes>(dirty_flag_factory(), input_factory())}
{
}

} // namespace Lab