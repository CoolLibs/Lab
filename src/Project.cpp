#include "Project.h"

namespace Lab {

Project::Project()
    : camera_manager{variable_registries.of<Cool::Variable<Cool::Camera>>().create_shared({})}
    , compositing_module{std::make_unique<Module_Compositing>(dirty_flag_factory(), input_factory())}
{
}

auto Project::is_empty() const -> bool
{
    return compositing_module->is_empty()
           && history.size() == 0;
}

} // namespace Lab