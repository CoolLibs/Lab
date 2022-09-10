#include "NodeFactory.h"
#include <Cool/File/File.h>
#include <Cool/Log/ToUser.h>
#include <filesystem>
#include <wafl/wafl.hpp>
#include "NodeTemplateParsing.h"

NodeFactory::NodeFactory(std::string_view nodes_folder_path)
    : _nodes_folder_path{nodes_folder_path}
{
    reload_templates();
}

void NodeFactory::clear_filter()
{
    _filter          = "";
    _focus_on_filter = true;
}

std::optional<Node> NodeFactory::imgui()
{
    std::optional<Node> res{};
    const auto          select_node_template = [&](const NodeTemplate& node_template) {
        res = NodeFactoryU::node_from_template(node_template);
    };
    const auto for_each_node_template = [&](std::function<void(const NodeTemplate&)>        callback,
                                            std::function<bool(const NodeTemplatesFolder&)> filter) {
        size_t nodes_counts = 0;
        for (const auto& folder : _folders)
        {
            if (filter(folder))
            {
                if (ImGui::BeginMenu(folder.name.c_str()))
                {
                    for (size_t i = 0; i < folder.nodes_count; ++i)
                    {
                        callback(_node_templates[nodes_counts]);
                        nodes_counts++;
                    }
                    ImGui::EndMenu();
                }
                else
                {
                    nodes_counts += folder.nodes_count;
                }
            }
            else
            {
                nodes_counts += folder.nodes_count;
            }
        }
    };

    // ImGui
    if (_focus_on_filter)
    {
        ImGui::SetKeyboardFocusHere();
        _focus_on_filter = false;
    }
    bool wants_to_select_first_node = ImGui::InputText("Search", &_filter, ImGuiInputTextFlags_EnterReturnsTrue);
    if (_filter.empty())
    {
        for_each_node_template([&](const NodeTemplate& node_template) {
            if (ImGui::MenuItem(node_template.name.c_str())) {
                        select_node_template(node_template);
                    } }, [&](const NodeTemplatesFolder&) { return true; });
    }
    else
    {
        size_t nodes_counts = 0;
        for (const auto& folder : _folders)
        {
            for (size_t i = 0; i < folder.nodes_count; ++i)
            {
                if (wafl::similarity_match({.input = _filter, .reference = _node_templates[nodes_counts].name}) >= wafl::Matches::Strongly)
                {
                    if (wants_to_select_first_node)
                    {
                        select_node_template(_node_templates[nodes_counts]);
                        wants_to_select_first_node = false;
                    }
                    if (ImGui::Selectable(_node_templates[nodes_counts].name.c_str()))
                    {
                        select_node_template(_node_templates[nodes_counts]);
                    }
                }
                nodes_counts++;
            }
        }
        for_each_node_template(
            [&](const NodeTemplate& node_template) {
            if (wants_to_select_first_node)
            {
                select_node_template(node_template);
                wants_to_select_first_node = false;
            }
            if (ImGui::Selectable(node_template.name.c_str()))
            {
                select_node_template(node_template);
            } },
            [&](const NodeTemplatesFolder& folder) { return wafl::similarity_match({.input = _filter, .reference = folder.name}) >= wafl::Matches::Strongly; }
        );
    }

    return res;
}

Node NodeFactoryU::node_from_template(const NodeTemplate& node_template)
{
    return Node{
        .node_template_name = node_template.name,
        .parameter_list     = Cool::ParameterList(node_template.parameters),
        .input_pins         = std::vector<PinSdfIn>(node_template.sdf_identifiers.size()),
    };
}

void NodeFactory::reload_templates()
{
    _node_templates.clear();
    _folders.clear();
    for (const auto& entry : std::filesystem::directory_iterator{_nodes_folder_path})
    {
        if (!entry.is_directory())
            continue;

        _folders.push_back({entry.path().stem().string(), 0});
        for (const auto& file : std::filesystem::directory_iterator{entry.path()})
        {
            if (!file.is_regular_file())
                continue;

            try
            {
                NodeTemplate node_template;
                node_template.name = file.path().stem().string();
                parse_node_template(node_template, Cool::File::to_string(file.path().string()));
                _node_templates.push_back(node_template);
                _folders.back().nodes_count++;
            }
            catch (const std::exception& e)
            {
                Cool::Log::ToUser::warning(
                    "is0::NodeFactory::" + file.path().stem().string(),
                    fmt::format("Failed to parse node from file '{}':\n{}", file.path().string(), e.what())
                );
            }
        }
    }
}

std::optional<FolderRange> NodeFactory::templates_from_folder(std::string_view folder_name) const
{
    size_t first = 0;
    for (const auto& folder : _folders)
    {
        if (folder.name == folder_name)
        {
            return FolderRange{
                first,
                first + folder.nodes_count - 1,
            };
        }
        else
        {
            first += folder.nodes_count;
        }
    }
    return std::nullopt;
}