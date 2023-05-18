#pragma once

#include <Cool/Dependencies/InputFactory_Ref.h>
#include <Cool/Nodes/Graph.h>
#include <Cool/Nodes/NodeId.h>
#include "Cool/Dependencies/Input.h"
#include "Cool/Nodes/GetNodeDefinition_Ref.h"
#include "Cool/Nodes/NodesLibrary.h"
#include "Dependencies/Ui.h"
#include "Module_Nodes/NodeDefinition.h"
#include "Node.h"
#include "NodeDefinition.h"

namespace Lab {

class NodesConfig {
public:
    NodesConfig(
        Cool::InputFactory_Ref                      input_factory,
        Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition,
        Ui_Ref                                      ui,
        Cool::NodeId&                               main_node_id,
        Cool::DirtyFlag const&                      rerender_flag,
        Cool::DirtyFlag const&                      regenerate_code_flag,
        Cool::Graph const&                          graph
    )
        : _input_factory{input_factory}
        , _get_node_definition{get_node_definition}
        , _ui{ui}
        , _main_node_id{main_node_id}
        , _rerender_flag{rerender_flag}
        , _regenerate_code_flag{regenerate_code_flag}
        , _graph{graph}
    {}

    auto        name(Cool::Node const&) const -> std::string;
    auto        category_name(Cool::Node const&) const -> std::string;
    void        imgui_above_node_pins(Cool::Node&, Cool::NodeId const&);
    void        imgui_below_node_pins(Cool::Node&, Cool::NodeId const&);
    void        imgui_in_inspector_above_node_info(Cool::Node&, Cool::NodeId const&);
    void        imgui_in_inspector_below_node_info(Cool::Node&, Cool::NodeId const&);
    auto        make_node(Cool::NodeDefinitionAndCategoryName const&) -> Node;
    void        update_node_with_new_definition(Cool::Node&, Cool::NodeDefinition const&, Cool::Graph&);
    static void widget_to_rename_node(Cool::Node&);

private:
    void main_node_toggle(Cool::NodeId const&);

private:
    Cool::InputFactory_Ref                      _input_factory;
    Cool::GetNodeDefinition_Ref<NodeDefinition> _get_node_definition;
    Ui_Ref                                      _ui;
    Cool::NodeId&                               _main_node_id;
    Cool::DirtyFlag                             _rerender_flag;
    Cool::DirtyFlag                             _regenerate_code_flag;
    Cool::Graph const&                          _graph;
};

} // namespace Lab
