#pragma once

#include <Cool/Gpu/DoubleBufferedRenderTarget.h>
#include <Cool/ImGui/IcoMoonCodepoints.h>
#include <Cool/ImGui/ImGuiWindow.h>
#include <Cool/ImGui/icon_fmt.h>
#include <Cool/Nodes/NodesGraph.h>
#include <Meshing/MeshingParams.h>
#include <Meshing/meshing_io.hpp>

namespace Lab {

class MeshingGui {
public:
    MeshingGui();

    void imgui_window(Meshing::MeshingParams& meshing_params, std::function<void(std::filesystem::path const&)> const& on_mesh_exported);

    void open_window();

private:
    [[nodiscard]] auto folder_path_for_mesh() const -> std::filesystem::path;
    void               set_file_name_to_an_unused_name();

private:
    // TODO [Meshing] : Add more supported export formats (obj, ply, gltf, etc.)
    std::filesystem::path                _file_name{"mesh(0)"};
    std::optional<std::filesystem::path> _folder_path_for_mesh;
    Meshing::MeshExportFormat            _export_format{Meshing::MeshExportFormat::PLY};
    Cool::ImGuiWindow                    _window{Cool::icon_fmt("Export a Mesh", ICOMOON_COG), Cool::ImGuiWindowConfig{.is_modal = true}};

    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Mesh Output Folder", _folder_path_for_mesh)
        );
    }
};

} // namespace Lab