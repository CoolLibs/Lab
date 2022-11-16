#include "NodeDefinition.h"
#include <Cool/String/String.h>

namespace Lab {

NodeDefinition::NodeDefinition(NodeDefinition_Data const& data)
    : _data{data}
{
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
}

} // namespace Lab
