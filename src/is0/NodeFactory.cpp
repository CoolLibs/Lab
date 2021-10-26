#include "NodeFactory.h"

std::optional<Node> NodeFactory::imgui()
{
    std::optional<Node> res{};
    for (const auto& node_template : _node_templates) {
        if (ImGui::Button(node_template.name.c_str())) {
            res = NodeFactoryU::node_from_template(node_template);
        }
    }
    return res;
}

Node NodeFactoryU::node_from_template(const NodeTemplate& node_template)
{
    return Node{.node_template_name = node_template.name,
                .parameter_list     = Cool::ParameterList(node_template.parameters),
                .input_pins         = std::vector<PinSdfIn>(node_template.sdf_identifiers.size())};
}