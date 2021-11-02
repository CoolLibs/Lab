#include "NodeFactory.h"
#include <Cool/File/File.h>
#include <Cool/Log/ToUser.h>
#include <filesystem>
#include "NodeTemplateParsing.h"

NodeFactory::NodeFactory(std::string_view nodes_folder_path)
{
    for (const auto& entry : std::filesystem::directory_iterator{nodes_folder_path}) {
        if (entry.is_regular_file()) {
            try {
                NodeTemplate node_template;
                node_template.name = entry.path().stem().string();
                parse_node_template(node_template, Cool::File::to_string(entry.path().string()));
                _node_templates.push_back(node_template);
            }
            catch (const std::exception& e) {
                Cool::Log::ToUser::warn("is0::NodeFactory", "Failed to parse node from file '{}':\n{}", entry.path().string(), e.what());
            }
        }
    }
}

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