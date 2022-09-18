#include "NodeLibrary.h"

namespace Lab {

auto NodeLibrary::get_definition(std::string_view definition_name) const -> const NodeDefinition*
{
    const auto it = std::find_if(_definitions.begin(), _definitions.end(), [&](const NodeDefinition& def) {
        return def.name == definition_name;
    });
    if (it == _definitions.end())
        return nullptr;

    return &*it;
}

void NodeLibrary::clear()
{
    _definitions.clear();
}

void NodeLibrary::add_nodes_from(std::filesystem::path)
{
    // TODO(JF)
}

} // namespace Lab