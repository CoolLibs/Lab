#pragma once

#include <Cool/Dependencies/InputFactory_Ref.h>
#include <Cool/Nodes/NodeId.h>
#include "Cool/Nodes/GetNodeDefinition_Ref.h"
#include "Dependencies/Ui.h"
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
        Cool::DirtyFlag const&                      regenerate_code_flag
    )
        : _input_factory{input_factory}
        , _get_node_definition{get_node_definition}
        , _ui{ui}
        , _main_node_id{main_node_id}
        , _rerender_flag{rerender_flag}
        , _regenerate_code_flag{regenerate_code_flag}
    {}

    using NodeT           = Node;
    using NodeDefinitionT = NodeDefinition;

    auto name(Node const&) const -> std::string;
    void imgui_node_body(Node&, Cool::NodeId const&) const;
    auto make_node(NodeDefinition const&) const -> Node;

private:
    Cool::InputFactory_Ref                               _input_factory;
    mutable Cool::GetNodeDefinition_Ref<NodeDefinition>  _get_node_definition;
    mutable Ui_Ref                                       _ui;
    Cool::NodeId&                                        _main_node_id;
    Cool::DirtyFlag                                      _rerender_flag;
    Cool::DirtyFlag                                      _regenerate_code_flag;
};

} // namespace Lab