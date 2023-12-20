#include "Node.h"
#include <stdexcept>
#include "Cool/String/String.h"
#include "PrimitiveType.h"

namespace Lab {

auto to_string(Node const& node) -> std::string
{
    return !node.name().empty() ? node.name() : node.definition_name();
}

auto Node::is_audio_node() const -> bool
{
    return Cool::String::contains(_id_names.definition_name, "Audio");
}

auto Node::is_particle_initializer() const -> bool
{
    return _particles_count.has_value();
}

} // namespace Lab

#include <Cool/Nodes/REGISTER_NODE_TYPE.h>
COOL_REGISTER_NODE_TYPE(Lab::Node);
