#pragma once
#include <CommandCore/CommandExecutor.h>
#include <Cool/Nodes/NodeId.h>
#include <Cool/Nodes/NodesGraph.h>
#include "Cool/Audio/AudioManager.h"
#include "Cool/Dependencies/DirtyFlag.h"
#include "Cool/Nodes/Editor.h"
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
        Cool::DirtyFlag const&                      rerender_flag,
        Cool::DirtyFlag const&                      regenerate_code_flag,
        Cool::NodesEditor&                          nodes_editor,
        Cool::AudioManager&                         audio_manager,
        CommandExecutor const&                      command_executor
    )
        : _get_node_definition{get_node_definition}
        , _get_node_category_config{get_node_category_config}
        , _ui{ui}
        , _main_node_id{main_node_id}
        , _rerender_flag{rerender_flag}
        , _regenerate_code_flag{regenerate_code_flag}
        , _nodes_editor{nodes_editor}
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
    void               on_link_created_between_existing_nodes(Cool::Link const&, Cool::LinkId const&);
    auto               add_node(Cool::NodeDefinitionAndCategoryName const&) -> Cool::NodeId;
    auto               add_node(Node const&) -> Cool::NodeId;
    void               on_node_created(Cool::Node&, Cool::NodeId const&, Cool::Pin const* pin_linked_to_new_node);
    [[nodiscard]] auto copy_nodes() const -> std::string;
    /// Returns true iff successfully pasted nodes
    auto        paste_nodes(std::string_view clipboard_content) -> bool;
    void        update_node_with_new_definition(Cool::Node&, Cool::NodeDefinition const&, Cool::NodesGraph&);
    void        update_node_with_new_definition(Node&, Cool::NodeDefinition const&, Cool::NodesGraph&);
    static void widget_to_rename_node(Cool::Node&);
    auto        maybe_disable_node_definition() const -> Cool::MaybeDisableNodeDefinition;

private:
    auto make_node(Cool::NodeDefinitionAndCategoryName const&) -> Node;
    void main_node_toggle(Cool::NodeId const&);
    void set_main_node_id(Cool::NodeId const& id);
    auto graph() const -> Cool::NodesGraph const& { return _nodes_editor.graph(); }
    auto graph() -> Cool::NodesGraph& { return _nodes_editor.graph(); }
    auto primary_dirty_flag(bool always_requires_shader_code_generation) const -> Cool::DirtyFlag const&;
    auto secondary_dirty_flag() const -> Cool::DirtyFlag const&;

private:
    Cool::GetNodeDefinition_Ref<NodeDefinition> _get_node_definition;
    Cool::GetNodeCategoryConfig_Ref             _get_node_category_config;
    Ui_Ref                                      _ui;
    Cool::NodeId const&                         _main_node_id;
    Cool::DirtyFlag                             _rerender_flag;
    Cool::DirtyFlag                             _regenerate_code_flag;
    Cool::NodesEditor&                          _nodes_editor;
    Cool::AudioManager&                         _audio_manager;
    CommandExecutor                             _command_executor;
};

} // namespace Lab
