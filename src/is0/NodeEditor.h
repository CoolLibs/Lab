#pragma once

#include <Cool/Path/Path.h>
#include "NodeFactory.h"
#include "NodeTree.h"
#include "UniqueImNodeContext.h"

class NodeEditor {
public:
    explicit NodeEditor(std::string_view factory_folder_path);
    void imgui_window();
    void update_templates_and_nodes();
    void open_menu();
    bool tree_has_changed();
    bool tree_is_valid() const { return _all_nodes_have_a_valid_template; }
    auto tree() const -> const NodeTree& { return _tree; }
    auto node_templates() const -> const std::vector<NodeTemplate>& { return _factory.templates(); }

private:
    bool wants_to_delete_selection() const;
    void on_tree_change();
    bool imgui_nodes_menu();
    bool handle_link_creation();
    bool handle_link_deletion();
    bool handle_node_deletion();

private:
    std::string         _factory_folder_path;
    UniqueImNodeContext _context;
    NodeFactory         _factory{_factory_folder_path};
    NodeTree            _tree;
    bool                _all_nodes_have_a_valid_template = true;
    bool                _tree_has_changed                = true;
    bool                _should_open_menu                = false;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Node Tree", _tree));
        update_templates_and_nodes();
    }
};