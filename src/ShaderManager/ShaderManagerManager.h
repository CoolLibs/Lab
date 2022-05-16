#pragma once

#include <is0/Is0.h>

class ShaderManagerManager {
public:
    void imgui_windows();

    // clang-format off
    ShaderManager& operator*() { return _is0;}
    ShaderManager* operator->() { return &operator*(); }
    // clang-format on

    void                             add_node(const Node& node) { _is0.add_node(node); }
    const std::vector<NodeTemplate>& nodes_templates() { return _is0.nodes_templates(); }

private:
    bool _use_nodes = true;
    Is0  _is0;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Is using nodes", _use_nodes),
            cereal::make_nvp("From Nodes", _is0));
    }
};