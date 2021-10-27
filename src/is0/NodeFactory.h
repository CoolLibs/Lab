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
                                   .parameters             = {Cool::Parameter::FloatDesc{
                                       .name          = "radius",
                                       .default_value = 1.f,
                                       .min_value     = 0.f,
                                       .max_value     = 10.f}},
                                   .sdf_identifiers        = {}});
        _node_templates.push_back({.name                   = "Cube",
                                   .vec3_input_declaration = "(vec3 pos)",
                                   .code_template          = R"( 
	vec3 q = abs(pos) - radius; 
	return length(max(q, 0.)) + min(max(q.x, max(q.y, q.z)), 0.); 
	)",
                                   .parameters             = {Cool::Parameter::FloatDesc{
                                       .name          = "radius",
                                       .default_value = 1.f,
                                       .min_value     = 0.f,
                                       .max_value     = 100.f}},
                                   .sdf_identifiers        = {}});
        _node_templates.push_back({.name                   = "Repeat",
                                   .vec3_input_declaration = "(vec3 pos)",
                                   .code_template          = "    return ${SDF}((fract(pos / cell_radius) - 0.5) * cell_radius);",
                                   .parameters             = {Cool::Parameter::FloatDesc{
                                       .name          = "cell_radius",
                                       .default_value = 5.f,
                                       .min_value     = 0.001f,
                                       .max_value     = 20.f}},
                                   .sdf_identifiers        = {"SDF"}});
        _node_templates.push_back({.name                   = "Twist",
                                   .vec3_input_declaration = "(vec3 p)",
                                   .code_template          = R"( 
    float c = cos(k*p.y);
    float s = sin(k*p.y);
    mat2  m = mat2(c,-s,s,c);
    vec3  q = vec3(m*p.xz,p.y);
    return ${SDF}(q);
	)",
                                   .parameters             = {Cool::Parameter::FloatDesc{
                                       .name          = "k",
                                       .default_value = 10.f,
                                       .min_value     = 0.001f,
                                       .max_value     = 20.f}},
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