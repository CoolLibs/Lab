#include "NodeDefinition.h"
#include <Cool/String/String.h>

namespace Lab {

NodeDefinition::NodeDefinition(NodeDefinition_Data const& data)
    : _data{data}
{
    // TODO(JF) Refactor that code duplicated with NodeInputDefinition.cpp
    // Check that the property names are valid, and remove the backticks from them.
    for (auto& prop : _data.properties)
    {
        std::visit([](auto& prop) {
            if (prop.name.size() < 2 /* Make sure indexing at `prop.name.size() - 1` is safe */
                || prop.name[0] != '`' || prop.name[prop.name.size() - 1] != '`')
            {
                throw std::runtime_error(fmt::format(
                    "All the PROPERTY names must start and end with backticks (`).\nName \"{}\" is invalid.", prop.name
                ));
            }

            prop.name = prop.name.substr(1, prop.name.size() - 2);
        },
                   prop);
    }

    for (auto& output_index_name : _data.output_indices)
    {
        if (output_index_name.size() < 2 /* Make sure indexing at `output_index_name.size() - 1` is safe */
            || output_index_name[0] != '`' || output_index_name[output_index_name.size() - 1] != '`')
        {
            throw std::runtime_error(fmt::format(
                "All the OUTPUT names must start and end with backticks (`).\nName \"{}\" is invalid.", output_index_name
            ));
        }

        output_index_name = output_index_name.substr(1, output_index_name.size() - 2);
    }
}

} // namespace Lab
