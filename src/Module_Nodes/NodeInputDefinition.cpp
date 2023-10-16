#include <Cool/String/String.h>
#include "NodeVariableDefinition.h"


namespace Lab {

NodeVariableDefinition::NodeVariableDefinition(NodeVariableDefinition_Data const& data)
    : _data{data}
{
    // TODO(JF) Refactor that code duplicated with NodeDefinition.cpp
    // Check that the name is valid, and remove the backticks from it.
    if (_data.name.size() < 2 /* Make sure indexing at `_data.name.size() - 1` is safe */
        || _data.name[0] != '\'' || _data.name[_data.name.size() - 1] != '\'')
    {
        throw std::runtime_error(fmt::format(
            "All the INPUT names must start and end with single quotes (').\nName \"{}\" is invalid.", _data.name
        ));
    }

    _data.name = _data.name.substr(1, _data.name.size() - 2);
}

} // namespace Lab
