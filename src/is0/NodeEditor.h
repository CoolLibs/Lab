#pragma once

#include <Cool/File/File.h>
#include "NodeFactory.h"
#include "NodeTree.h"
#include "UniqueImNodeContext.h"

class NodeEditor {
public:
    NodeEditor();

    void imgui_window();
    bool imgui_make_node();
    auto shader_code() const -> const std::string& { return _shader_code; }

    /// Calls the callback, and will call it again whenever the shader code changes
    void subscribe_to_shader_code_changes(std::function<void(const std::string&)> callback);

    void update_templates_and_nodes();

private:
    void handle_link_creation();
    void handle_link_deletion();
    void handle_node_deletion();
    void update_shader_code();

private:
    UniqueImNodeContext _context;
    NodeFactory         _factory{Cool::File::root_dir() + "/is0 nodes"};
    std::string         _shader_code;
    NodeTree            _tree;
    bool                _all_nodes_have_a_valid_template = true;

    std::vector<std::function<void(const std::string&)>> _on_shader_code_change;

private:
    //Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Node Tree", _tree));
        update_templates_and_nodes();
        update_shader_code();
    }
};