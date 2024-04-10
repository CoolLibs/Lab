#pragma once

#include <array>
#include <glm/glm.hpp>

namespace Meshing {

struct Mesh {
    std::vector<glm::vec3>  vertices;
    std::vector<glm::uvec3> indices;
};

struct GridCell {
    std::array<glm::vec3, 8> vertices;
    std::array<float, 8>     sdf_values;
};

using Triangle      = std::array<glm::vec3, 3>;
using TriangleIndex = glm::uvec3;
} // namespace Meshing