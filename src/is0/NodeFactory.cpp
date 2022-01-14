#include "NodeFactory.h"
#include <Cool/File/File.h>
#include <Cool/Log/ToUser.h>
#include <filesystem>
#include "NodeTemplateParsing.h"

NodeFactory::NodeFactory(std::string_view nodes_folder_path)
    : _nodes_folder_path{nodes_folder_path}
{
    reload_templates();
}

std::optional<Node> NodeFactory::imgui()
{
    std::optional<Node> res{};
    size_t              nodes_counts = 0;
    for (const auto& folder : _folders) {
        if (ImGui::BeginMenu(folder.name.c_str())) {
            for (size_t i = 0; i < folder.nodes_count; ++i) {
                if (ImGui::MenuItem(_node_templates[nodes_counts].name.c_str())) {
                    res = NodeFactoryU::node_from_template(_node_templates[nodes_counts]);
                }
                nodes_counts++;
            }
            ImGui::EndMenu();
        }
        else {
            nodes_counts += folder.nodes_count;
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

void NodeFactory::reload_templates()
{
    _node_templates.clear();
    _folders.clear();
    for (const auto& entry : std::filesystem::directory_iterator{_nodes_folder_path}) {
        if (entry.is_directory()) {
            _folders.push_back({entry.path().stem().string(), 0});
            for (const auto& file : std::filesystem::directory_iterator{entry.path()}) {
                if (file.is_regular_file()) {
                    try {
                        NodeTemplate node_template;
                        node_template.name = file.path().stem().string();
                        parse_node_template(node_template, Cool::File::to_string(file.path().string()));
                        _node_templates.push_back(node_template);
                        _folders.back().nodes_count++;
                    }
                    catch (const std::exception& e) {
                        Cool::Log::ToUser::warn("is0::NodeFactory::" + file.path().stem().string(), "Failed to parse node from file '{}':\n{}", file.path().string(), e.what());
                    }
                }
            }
        }
    }
}