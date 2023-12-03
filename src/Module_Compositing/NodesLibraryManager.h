#pragma once
#include <Dependencies/UpdateContext_Ref.h>
#include "Cool/Nodes/NodesFolderWatcher.h"
#include "Cool/Nodes/NodesLibrary.h"
#include "Cool/Path/Path.h"
#include "NodesConfig.h"

namespace Lab {

class NodesLibraryManager {
public:
    void update(UpdateContext_Ref, Cool::DirtyFlag& regenerate_code_flag, Cool::Graph&, NodesConfig const&);

    auto library() const -> auto const& { return _nodes_library; }
    auto library() -> auto& { return _nodes_library; }

private:
    Cool::NodesLibrary       _nodes_library{};
    Cool::NodesFolderWatcher _nodes_folder_watcher{Cool::Path::root() / "Nodes", ".clbnode"};
};

} // namespace Lab