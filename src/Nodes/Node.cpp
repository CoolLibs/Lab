#include "Node.h"
#include "Cool/String/String.h"

namespace Lab {

auto to_string(Node const& node) -> std::string
{
    return !node.name().empty() ? node.name() : node.definition_name();
}

auto Node::is_audio_node() const -> bool
{
    return Cool::String::contains(_d.id_names.definition_name, "Audio");
}

auto Node::as_pod() const -> NodeAsPOD
{
    auto pod = NodeAsPOD{_d};
    for (auto const& input : _value_inputs)
        pod.value_inputs.push_back(std::visit([](auto&& input) -> Cool::AnyVariable { return input.variable(); }, input));
    return pod;
}

} // namespace Lab

#include <Cool/Nodes/REGISTER_NODE_TYPE.h>
COOL_REGISTER_NODE_TYPE(Lab::Node);
