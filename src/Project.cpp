#include "Project.h"

namespace Lab {

Project::Project(Cool::RenderView& nodes_view)
    : camera_manager{variable_registries.of<Cool::Variable<Cool::Camera>>().create_shared({})}
    , nodes_module{std::make_unique<Module_Nodes>(dirty_flag_factory(), input_factory())}
{
    camera_manager.is_editable_in_view() = false;
    clock.pause();
    camera_manager.hook_events(nodes_view.mouse_events(), variable_registries, command_executor(), [this]() { trigger_rerender(); });
    hook_camera2D_events(
        nodes_view.mouse_events(),
        _camera2D.value(),
        [this]() { trigger_rerender(); },
        [this]() { return !_is_camera_2D_editable_in_view; }
    );
}

void Project::trigger_rerender()
{
    set_dirty_flag()(nodes_module->dirty_flag());
}

} // namespace Lab