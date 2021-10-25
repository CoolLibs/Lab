#pragma once

#include <imgui-node-editor/imgui_node_editor.h>
namespace ed = ax::NodeEditor;

struct Link {
    ed::LinkId id;
    ed::PinId  from_pin_id;
    ed::PinId  to_pin_id;
};
