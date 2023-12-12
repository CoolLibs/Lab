#pragma once

#include <Cool/Nodes/NodeId.h>
#include "NodeDefinition.h"

namespace Lab {

auto module_texture_name(NodeDefinition const& definition, Cool::NodeId const& id) -> std::string;

}
