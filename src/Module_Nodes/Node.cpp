#include "Node.h"
#include <stdexcept>
#include "Module_Nodes/PrimitiveType.h"

namespace Lab {

auto to_string(Node const& node) -> std::string
{
    return !node.name().empty() ? node.name() : node.definition_name();
}

} // namespace Lab

#include <Cool/Nodes/REGISTER_NODE_TYPE.h>
COOL_REGISTER_NODE_TYPE(Lab::Node);