#pragma once

#include <Cool/Path/Path.h>
#include "NodeFactory.h"
#include "NodeTree.h"
#include "UniqueImNodeContext.h"

class NodeEditor {
public:
    explicit NodeEditor(std::string_view nodes_folder_path);
    void imgui_window();
    void update_templates_and_nodes();
    void ask_to_open_nodes_menu();
    bool tree_has_changed();
    bool tree_is_valid() const { return _all_nodes_have_a_valid_template; }
    auto tree() const -> const NodeTree& { return _tree; }
    auto node_templates() const -> const std::vector<NodeTemplate>& { return _factory.templates(); }
    void add_node(const Node& node) { _tree.add_node(node); }
    void generate_a_random_scene();

private:
    auto should_open_nodes_menu() -> bool;
    void open_nodes_menu();
    bool wants_to_delete_selection() const;
    void on_tree_change();
    bool imgui_nodes_menu();
    bool handle_link_creation();
    bool handle_link_deletion();
    bool handle_node_deletion();

private:
    UniqueImNodeContext _context;
    NodeFactory         _factory;
    NodeTree            _tree;
    bool                _all_nodes_have_a_valid_template = true;
    bool                _tree_has_changed                = true;
    bool                _should_open_nodes_menu                = false;
    ImVec2              _next_node_position              = {0.f, 0.f};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        archive(cereal::make_nvp("Node Tree", _tree),
                cereal::make_nvp("Editor State", std::string{ImNodes::SaveCurrentEditorStateToIniString()}));
    }
    template<class Archive>
    void load(Archive& archive)
    {
        std::string editor_state;
        archive(cereal::make_nvp("Node Tree", _tree),
                cereal::make_nvp("Editor State", editor_state));
        ImNodes::LoadCurrentEditorStateFromIniString(editor_state.c_str(), editor_state.size());
        update_templates_and_nodes();
    }
};