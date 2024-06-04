#include "meshing_io.hpp"
#include <Cool/Path/Path.h>
#include <array>
#include <fstream>

namespace Meshing {

auto enum_name(MeshExportFormat const& export_format) -> std::string
{
    switch (export_format)
    {
    case MeshExportFormat::PLY: return "PLY";
    }
    throw std::runtime_error("Unknown MeshExportFormat");
}

// TODO: move in Cool::ImGuiExtras and generalize for enums (using magic_enum)
void imgui_combo(MeshExportFormat& export_format)
{
    static const std::array<const char*, 1> items{"PLY"};
    unsigned int                            item_current{static_cast<unsigned int>(export_format)};
    if (ImGui::BeginCombo("Export Format", items[item_current]))
    {
        for (unsigned int i{0}; i < items.size(); i++)
        {
            bool const is_selected{(item_current == i)};
            if (ImGui::Selectable(items[i], is_selected))
            {
                item_current = i;
            }
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    export_format = static_cast<MeshExportFormat>(item_current);
}

void write_to_file(Mesh const& mesh, std::filesystem::path const& path, MeshExportFormat const& export_format)
{
    switch (export_format)
    {
    case MeshExportFormat::PLY:
        write_to_ply(mesh, path);
        break;

    default:
        throw std::runtime_error("Unknown MeshExportFormat");
    }
}

void write_to_ply(Mesh const& mesh, std::filesystem::path const& path)
{
    std::ofstream outputFile{};

    outputFile.open(path);

    outputFile << "ply\n";
    outputFile << "format ascii 1.0\n";
    outputFile << "element vertex " << mesh.vertices.size() << "\n";
    outputFile << "property float32 x\n";
    outputFile << "property float32 y\n";
    outputFile << "property float32 z\n";
    outputFile << "element face " << mesh.indices.size() << "\n";
    outputFile << "property list uint8 int32 vertex_indices\n";
    outputFile << "end_header\n";

    for (auto const& vertex : mesh.vertices)
    {
        outputFile << vertex.x << " " << vertex.y << " " << vertex.z << "\n";
    }

    for (auto const& face_indices : mesh.indices)
    {
        outputFile << 3 << " " << face_indices.x << " " << face_indices.y << " " << face_indices.z << "\n";
    }
}

}; // namespace Meshing