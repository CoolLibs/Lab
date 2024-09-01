#include "ModulesGraphNode.hpp"

namespace Lab {

auto ModulesGraphNode::needs_to_rerender() const -> bool
{
    return module->needs_to_rerender() || std::any_of(dependencies.begin(), dependencies.end(), [&](auto&& node) {
               return node->module->needs_to_rerender();
           });
}

} // namespace Lab