#pragma once
#include <CommandCore/CommandExecutor.h>
#include <Cool/Nodes/NodeId.h>
#include <Cool/Nodes/NodesGraph.h>
#include "Cool/Audio/AudioManager.h"
#include "Cool/Nodes/GetNodeCategoryConfig.h"
#include "Cool/Nodes/GetNodeDefinition_Ref.h"
#include "Cool/Nodes/NodesLibrary.h"
#include "Cool/StrongTypes/Color.h"
#include "Dependencies/Ui.h"
#include "Node.h"
#include "NodeDefinition.h"

namespace Lab {

class NodesConfig {
public:
    NodesConfig(
        Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition,
        Cool::GetNodeCategoryConfig_Ref             get_node_category_config,
        Ui_Ref                                      ui,
        Cool::NodeId&                               main_node_id,
        Cool::NodeId&                               node_we_might_want_to_restore_as_main_node_id,
        Cool::DirtyFlag const&                      rerender_flag,
        Cool::DirtyFlag const&                      regenerate_code_flag,
        Cool::NodesGraph const&                     graph,
        Cool::AudioManager&                         audio_manager,
        CommandExecutor const&                      command_executor
    )
        : _get_node_definition{get_node_definition}
        , _get_node_category_config{get_node_category_config}
        , _ui{ui}
        , _main_node_id{main_node_id}
        , _node_we_might_want_to_restore_as_main_node_id{node_we_might_want_to_restore_as_main_node_id}
        , _rerender_flag{rerender_flag}
        , _regenerate_code_flag{regenerate_code_flag}
        , _graph{graph}
        , _audio_manager{audio_manager}
        , _command_executor{command_executor}
    {}

    auto               name(Cool::Node const&) const -> std::string;
    auto               category_name(Cool::Node const&) const -> std::string;
    void               imgui_above_node_pins(Cool::Node&, Cool::NodeId const&);
    void               imgui_below_node_pins(Cool::Node&, Cool::NodeId const&);
    void               imgui_in_inspector_above_node_info(Cool::Node&, Cool::NodeId const&);
    void               imgui_in_inspector_below_node_info(Cool::Node&, Cool::NodeId const&);
    [[nodiscard]] auto node_color(Cool::Node const&, Cool::NodeId const&) const -> Cool::Color;
    [[nodiscard]] auto pin_color(Cool::Pin const&, size_t pin_index, Cool::Node const&, Cool::NodeId const&) const -> Cool::Color;
    void               on_node_created(Cool::Node&, Cool::NodeId const&, Cool::Pin const* pin_linked_to_new_node);
    void               on_link_created_between_existing_nodes(Cool::Link const&, Cool::LinkId const&);
    auto               make_node(Cool::NodeDefinitionAndCategoryName const&) -> Node;
    void               update_node_with_new_definition(Cool::Node&, Cool::NodeDefinition const&, Cool::NodesGraph&);
    static void        widget_to_rename_node(Cool::Node&);
    auto               maybe_disable_node_definition() const -> Cool::MaybeDisableNodeDefinition;

private:
    void main_node_toggle(Cool::NodeId const&);
    void set_main_node_id(Cool::NodeId const& id, bool keep_node_we_might_want_to_restore_as_main_node_id = false);

private:
    Cool::GetNodeDefinition_Ref<NodeDefinition> _get_node_definition;
    Cool::GetNodeCategoryConfig_Ref             _get_node_category_config;
    Ui_Ref                                      _ui;
    Cool::NodeId&                               _main_node_id;
    Cool::NodeId&                               _node_we_might_want_to_restore_as_main_node_id;
    Cool::DirtyFlag                             _rerender_flag;
    Cool::DirtyFlag                             _regenerate_code_flag;
    Cool::NodesGraph const&                     _graph;
    Cool::AudioManager&                         _audio_manager;
    CommandExecutor                             _command_executor;
};

} // namespace Lab
