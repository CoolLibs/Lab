#include "NodeDefinition.h"
#include <Cool/String/String.h>
#include "Cool/Variables/PresetManager.h"
#include "NodeDefinition.h"
#include "tl/expected.hpp"

namespace Lab {

NodeDefinition::NodeDefinition(NodeDefinition_Data const& data, Cool::PresetsPaths const& presets_paths)
    : _data{data}
    , _presets_manager(std::make_shared<Cool::PresetManager>(presets_paths))
{}

auto NodeDefinition::make(NodeDefinition_Data const& data, Cool::PresetsPaths const& presets_paths)
    -> tl::expected<NodeDefinition, std::string>
{
    auto def = NodeDefinition{data, presets_paths};
    // TODO(JF) Refactor that code duplicated with NodeInputDefinition.cpp
    // Check that the property names are valid, and remove the single quotes from them.
    for (auto& prop : def._data.input_values)
    {
        auto const err = std::visit([](auto& prop) -> std::optional<std::string> {
            if (prop.var_data.name.size() < 2 /* Make sure indexing at `prop.name.size() - 1` is safe */
                || prop.var_data.name[0] != '\'' || prop.var_data.name[prop.var_data.name.size() - 1] != '\'')
            {
                return fmt::format(
                    "All the PROPERTY names must start and end with single quotes (').\nName \"{}\" is invalid.", prop.var_data.name
                );
            }

            prop.var_data.name = prop.var_data.name.substr(1, prop.var_data.name.size() - 2);
            return std::nullopt;
        },
                                    prop);

        if (err)
            return tl::make_unexpected(*err);
    }

    for (auto& output_index_name : def._data.output_indices)
    {
        if (output_index_name.size() < 2 /* Make sure indexing at `output_index_name.size() - 1` is safe */
            || output_index_name[0] != '\'' || output_index_name[output_index_name.size() - 1] != '\'')
        {
            return tl::make_unexpected(fmt::format(
                "All the OUTPUT names must start and end with single quotes (').\nName \"{}\" is invalid.", output_index_name
            ));
        }

        output_index_name = output_index_name.substr(1, output_index_name.size() - 2);
    }

    return def;
}

} // namespace Lab
