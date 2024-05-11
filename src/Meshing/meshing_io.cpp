#include "meshing_io.hpp"
#include <Cool/Path/Path.h>
#include <fstream>

namespace Meshing {

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