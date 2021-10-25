#pragma once

#include "Node.h"
#include "NodeTemplate.h"

class NodeFactory {
public:
    NodeFactory()
    {
        _node_templates.push_back({.name            = "Sphere",
                                   .code_template   = "return length(pos) - radius;",
                                   .parameters      = {},
                                   .sdf_identifiers = {}});
        _node_templates.push_back({.name            = "Cube",
                                   .code_template   = "return length(pos) * 2.;",
                                   .parameters      = {},
                                   .sdf_identifiers = {}});
        _node_templates.push_back({.name            = "Repeat",
                                   .code_template   = "return length(pos) * 2.;",
                                   .parameters      = {},
                                   .sdf_identifiers = {"SDF"}});
        _node_templates.push_back({.name            = "Intersection",
                                   .code_template   = "return max(${SDF1}(pos), ${SDF2}(pos));",
                                   .parameters      = {},
                                   .sdf_identifiers = {"SDF1", "SDF2"}});
    }
    std::optional<Node>              imgui();
    const std::vector<NodeTemplate>& templates() const { return _node_templates; }

private:
    std::vector<NodeTemplate> _node_templates;
};

namespace NodeFactoryU {

Node node_from_template(const NodeTemplate& node_template);

}