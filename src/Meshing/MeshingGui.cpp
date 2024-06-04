#include "MeshingGui.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/Path/Path.h>
#include <imgui.h>
#include <filesystem>
#include <string>

namespace Lab {

auto meshing_export_mesh_Params::folder_path_for_mesh() const -> std::filesystem::path
{
    return std::filesystem::weakly_canonical(custom_folder_path.value_or(Cool::Path::project_folder().value_or(Cool::Path::user_data()) / "mesh"));
}

void meshing_export_mesh_Params::set_file_name_to_an_unused_name()
{
    // Trick to be able to use find_available_name method but store the result without extension
    file_name = Cool::File::without_extension(Cool::File::find_available_name(folder_path_for_mesh(), file_name, Meshing::enum_name(format)));
}

auto meshing_export_mesh_Params::file_path() const -> std::filesystem::path
{
    return folder_path_for_mesh() / (file_name + "." + Meshing::enum_name(format));
}

MeshingGui::MeshingGui()
{
    _window.on_open().subscribe([&](auto&&) {
        _export_mesh_params.set_file_name_to_an_unused_name();
    });
}

void MeshingGui::open_window()
{
    _window.open();
}

void MeshingGui::imgui_window(Meshing::MeshingParams& meshing_params, std::function<void(meshing_export_mesh_Params const&)> const& on_mesh_exported)
{
    _window.show([&]() {
        meshing_params.imgui();
        ImGui::Separator();
        {
            std::filesystem::path path{_export_mesh_params.folder_path_for_mesh()};
            if (Cool::ImGuiExtras::folder("Folder", &path))
            {
                _export_mesh_params.custom_folder_path = path;
                _export_mesh_params.set_file_name_to_an_unused_name();
            }
        }
        ImGui::InputText("File Name", &_export_mesh_params.file_name);

        Meshing::imgui_combo("Export Format", _export_mesh_params.format);
        if (std::filesystem::exists(_export_mesh_params.file_path()))
        {
            Cool::ImGuiExtras::warning_text(Cool::icon_fmt("A file with this name and format already exists in the folder. The file will be overwritten.", ICOMOON_WARNING).c_str());
        }

        if (ImGui::Button(Cool::icon_fmt("Export Mesh", ICOMOON_COGS).c_str()))
        {
            _window.close();
            on_mesh_exported(_export_mesh_params);
        }
    });
}

} // namespace Lab