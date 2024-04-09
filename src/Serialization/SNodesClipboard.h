#pragma once
#include "Nodes/NodesClipboard.h"

namespace Lab {

auto string_from_nodes_clipboard(NodesClipboard const&) -> std::string;
auto string_to_nodes_clipboard(std::string const&) -> NodesClipboard;

} // namespace Lab