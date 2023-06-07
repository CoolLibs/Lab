#include "NodeDefinition.h"
#include <Cool/String/String.h>
#include "Cool/Variables/PresetManager.h"
#include "Module_Nodes/NodeDefinition.h"
#include "tl/expected.hpp"

namespace Lab {

NodeDefinition::NodeDefinition(NodeDefinition_Data const& data, std::filesystem::path const& presets_file_path)
    : _data{data}
    , _presets_manager(std::make_shared<Cool::PresetManager>(presets_file_path))
{}

auto NodeDefinition::make(NodeDefinition_Data const& data, std::filesystem::path const& presets_file_path)
    -> tl::expected<NodeDefinition, std::string>
{
    auto def = NodeDefinition{data, presets_file_path};
    // TODO(JF) Refactor that code duplicated with NodeInputDefinition.cpp
    // Check that the property names are valid, and remove the backticks from them.
    for (auto& value_input : def._data.value_inputs)
    {
        auto const err = std::visit([](auto& cool_input) -> std::optional<std::string> {
            if (cool_input.name.size() < 2 /* Make sure indexing at `cool_input.name.size() - 1` is safe */
                || cool_input.name[0] != '`' || cool_input.name[cool_input.name.size() - 1] != '`')
            {
                return fmt::format(
                    "All the PROPERTY names must start and end with backticks (`).\nName \"{}\" is invalid.", cool_input.name
                );
            }

            cool_input.name = cool_input.name.substr(1, cool_input.name.size() - 2);
            return std::nullopt;
        },
                                    value_input.input_def());

        if (err)
            return tl::make_unexpected(*err);
    }

    for (auto& output_index_name : def._data.output_indices)
    {
        if (output_index_name.size() < 2 /* Make sure indexing at `output_index_name.size() - 1` is safe */
            || output_index_name[0] != '`' || output_index_name[output_index_name.size() - 1] != '`')
        {
            return tl::make_unexpected(fmt::format(
                "All the OUTPUT names must start and end with backticks (`).\nName \"{}\" is invalid.", output_index_name
            ));
        }

        output_index_name = output_index_name.substr(1, output_index_name.size() - 2);
    }

    return def;
}

} // namespace Lab
