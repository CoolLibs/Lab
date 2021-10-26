#pragma once

#include "Node.h"
#include "NodeTemplate.h"

class NodeFactory {
public:
    NodeFactory()
    {
        _node_templates.push_back({.name                   = "Sphere",
                                   .vec3_input_declaration = "(vec3 pos)",
                                   .code_template          = "    return length(pos) - radius;",
                                   .parameters             = {Cool::Parameter::FloatDesc{"radius"}},
                                   .sdf_identifiers        = {}});
        _node_templates.push_back({.name                   = "Cube",
                                   .vec3_input_declaration = "(vec3 pos)",
                                   .code_template          = R"( 
	vec3 q = abs(pos) - 1.; 
	return length(max(q, 0.)) + min(max(q.x, max(q.y, q.z)), 0.); 
	)",
                                   .parameters             = {},
                                   .sdf_identifiers        = {}});
        _node_templates.push_back({.name                   = "Repeat",
                                   .vec3_input_declaration = "(vec3 pos)",
                                   .code_template          = "    return ${SDF}((fract(pos / extent) - 0.5) * extent);",
                                   .parameters             = {Cool::Parameter::FloatDesc{"extent"}},
                                   .sdf_identifiers        = {"SDF"}});
        _node_templates.push_back({.name                   = "Intersection",
                                   .vec3_input_declaration = "(vec3 pos)",
                                   .code_template          = "    return max(${SDF1}(pos), ${SDF2}(pos));",
                                   .parameters             = {},
                                   .sdf_identifiers        = {"SDF1", "SDF2"}});
    }
    std::optional<Node>              imgui();
    const std::vector<NodeTemplate>& templates() const { return _node_templates; }

private:
    std::vector<NodeTemplate> _node_templates;
};

namespace NodeFactoryU {

Node node_from_template(const NodeTemplate& node_template);

}