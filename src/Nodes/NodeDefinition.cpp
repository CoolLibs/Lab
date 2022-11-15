#include "NodeDefinition.h"

namespace Lab {

NodeDefinition::NodeDefinition(NodeDefinition_Data const& data)
    : _data{data}
{
    // Check that the property names are valid.
    for (auto const& prop : properties())
    {
        std::string const& name = std::visit([](auto&& prop) { return prop.name; }, prop);
        if (name.size() < 2 /* Make sure indexing at `name.size() - 1` is safe */
            || name[0] != '"' || name[name.size() - 1] != '"')
        {
            throw std::runtime_error(fmt::format(
                "All the PROPERTY names must start and end with quotes (\").\nName '{}' is invalid.", name
            ));
        }
    }
}

} // namespace Lab
