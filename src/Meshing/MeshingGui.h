#pragma once

#include <Cool/Gpu/DoubleBufferedRenderTarget.h>
#include <Cool/ImGui/IcoMoonCodepoints.h>
#include <Cool/ImGui/ImGuiWindow.h>
#include <Cool/ImGui/icon_fmt.h>
#include <Cool/Nodes/NodesGraph.h>
#include <Meshing/MeshingParams.h>
#include <Meshing/meshing_io.hpp>

namespace Lab {

struct meshing_export_mesh_Params {
    std::string                          file_name;
    Meshing::MeshExportFormat            format;
    std::optional<std::filesystem::path> custom_folder_path;

    [[nodiscard]] auto folder_path_for_mesh() const -> std::filesystem::path;
    void               set_file_name_to_an_unused_name();

    [[nodiscard]] auto file_path() const -> std::filesystem::path;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("File Name", file_name),
            cereal::make_nvp("Export Format", Meshing::enum_name(format)),
            cereal::make_nvp("Folder Path", folder_path_for_mesh())
        );
    }
};

class MeshingGui {
public:
    MeshingGui();

    void imgui_window(Meshing::MeshingParams& meshing_params, std::function<void(meshing_export_mesh_Params const&)> const& on_mesh_exported);

    void open_window();

private:
    // TODO [Meshing] : Add more supported export formats (obj, ply, gltf, etc.)
    meshing_export_mesh_Params _export_mesh_params{"mesh(0)", Meshing::MeshExportFormat::PLY, std::nullopt};
    Cool::ImGuiWindow          _window{Cool::icon_fmt("Export a Mesh", ICOMOON_COG), Cool::ImGuiWindowConfig{.is_modal = true}};

    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Mesh Export Params", _export_mesh_params)
        );
    }
};

} // namespace Lab