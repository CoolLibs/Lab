#pragma once

#include <Cool/Dependencies/InputFactory_Ref.h>
#include <Cool/Nodes/NodeId.h>
#include "Dependencies/Ui.h"
#include "Node.h"
#include "NodeDefinition.h"

namespace Lab {

class NodesConfig {
public:
    NodesConfig(
        Cool::InputFactory_Ref const& input_factory,
        Ui_Ref                        ui,
        Cool::NodeId&                 main_node_id,
        Cool::DirtyFlag const&        rerender_flag,
        Cool::DirtyFlag const&        regenerate_code_flag
    )
        : _input_factory{input_factory}
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
    std::reference_wrapper<Cool::InputFactory_Ref const> _input_factory;
    mutable Ui_Ref                                       _ui;
    Cool::NodeId&                                        _main_node_id;
    Cool::DirtyFlag                                      _rerender_flag;
    Cool::DirtyFlag                                      _regenerate_code_flag;
};

} // namespace Lab
