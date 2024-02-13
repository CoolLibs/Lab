#pragma once

namespace Lab {

class Camera3DManager;
class ModulesGraph;

class MakeReversibleCommandContext_Ref {
public:
    auto camera_manager() const -> Camera3DManager const& { return _data.camera_3D_manager; }
    auto modules_graph() const -> ModulesGraph const& { return _data.modules_graph; }

    struct Data { // We wrap our members in a struct to get a constructor automatically
        std::reference_wrapper<Camera3DManager const> camera_3D_manager;
        std::reference_wrapper<ModulesGraph const>    modules_graph;
    };
    explicit MakeReversibleCommandContext_Ref(Data data)
        : _data{data} {}

private:
    Data _data;
};

} // namespace Lab
