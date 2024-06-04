#include "MeshingGui.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/Path/Path.h>
#include <filesystem>

namespace Lab {

MeshingGui::MeshingGui()
{
    _window.on_open().subscribe([&](auto&&) {
        set_file_name_to_an_unused_name();
    });
}

auto MeshingGui::folder_path_for_mesh() const -> std::filesystem::path
{
    return std::filesystem::weakly_canonical(_folder_path_for_mesh.value_or(Cool::Path::project_folder().value_or(Cool::Path::user_data()) / "mesh"));
}

void MeshingGui::set_file_name_to_an_unused_name()
{
    _file_name = Cool::File::find_available_name(folder_path_for_mesh(), _file_name, "." + Meshing::enum_name(_export_format));
}

void MeshingGui::open_window()
{
    _window.open();
}

void MeshingGui::imgui_window(Meshing::MeshingParams& meshing_params, std::function<void(std::filesystem::path const&)> const& on_mesh_exported)
{
    _window.show([&]() {
        meshing_params.imgui();

        // File and Folders
        Cool::ImGuiExtras::file("File Name", &_file_name, {}, {}, false /*No dialog button*/);
        if (std::filesystem::exists(folder_path_for_mesh() / _file_name))
        {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), Cool::icon_fmt("A file with this name already exists in the folder.", ICOMOON_WARNING).c_str());
        }
        {
            std::filesystem::path path{folder_path_for_mesh()};
            if (Cool::ImGuiExtras::folder("Folder", &path))
            {
                _folder_path_for_mesh = path;
                set_file_name_to_an_unused_name();
            }
        }

        Meshing::imgui_combo(_export_format);

        if (ImGui::Button(Cool::icon_fmt("Export Mesh", ICOMOON_COGS).c_str()))
        {
            _window.close();

            std::filesystem::path const path{folder_path_for_mesh()};
            if (!std::filesystem::exists(path))
            {
                std::filesystem::create_directory(path);
            }
            on_mesh_exported(path / _file_name.replace_extension("." + Meshing::enum_name(_export_format)));
        }
    });
}

} // namespace Lab