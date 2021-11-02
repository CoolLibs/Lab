#pragma once

#include <Cool/File/File.h>
#include "EdUniqueContext.h"
#include "Link.h"
#include "NodeFactory.h"

class NodeEditor {
public:
    NodeEditor();

    void imgui_window();
    bool imgui_make_node();
    auto shader_code() const -> const std::string& { return _shader_code; }

    /// Calls the callback, and will call it again whenever the shader code changes
    void subscribe_to_shader_code_changes(std::function<void(const std::string&)> callback);

private:
    void handle_link_creation();
    void handle_link_deletion();
    void handle_node_deletion();
    void update_shader_code();
    void delete_node(ed::NodeId id);

private:
    EdUniqueContext   _context;
    NodeFactory       _factory{Cool::File::root_dir() + "/is0 nodes"};
    std::vector<Node> _nodes;
    std::vector<Link> _links;
    std::string       _shader_code;

    std::vector<std::function<void(const std::string&)>> _on_shader_code_change;

private:
    //Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Nodes", _nodes),
                cereal::make_nvp("Links", _links));
        update_shader_code();
    }
};