#include "NodeManager.h"
#include "CodeGen.h"

bool NodeManager::imgui()
{
    const std::optional<Node> node = _factory.imgui();
    if (node.has_value()) {
        _nodes.push_back(*node);
        return true;
    }
    else {
        return false;
    }
}