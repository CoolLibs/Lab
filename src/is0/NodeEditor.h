#pragma once

#include <Cool/Path/Path.h>
#include "NodeFactory.h"
#include "NodeTree.h"
#include "UniqueImNodeContext.h"

class NodeEditor {
public:
    void        imgui_window();
    void        update_templates_and_nodes();
    void        open_menu();
    bool        tree_has_changed();
    bool        tree_is_valid() const { return _all_nodes_have_a_valid_template; }
    auto        tree() const -> const NodeTree& { return _tree; }
    auto        node_templates() const -> const std::vector<NodeTemplate>& { return _factory.templates(); }
    inline void add_node(const Node& node) { _tree.add_node(node); }

private:
    bool wants_to_delete_selection() const;
    void on_tree_change();
    bool imgui_nodes_menu();
    bool handle_link_creation();
    bool handle_link_deletion();
    bool handle_node_deletion();

private:
    UniqueImNodeContext _context;
    NodeFactory         _factory{Cool::Path::root() + "/is0 nodes"};
    NodeTree            _tree;
    bool                _all_nodes_have_a_valid_template = true;
    bool                _tree_has_changed                = true;
    bool                _should_open_menu                = false;
    ImVec2              _next_node_position              = {0.f, 0.f};

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