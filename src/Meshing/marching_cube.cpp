#include "marching_cube.hpp"
#include <array>
#include <unordered_map>
#include <utility>
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

namespace Meshing {

/**
 * Given a `cell`, calculate its cube index
 * The cube index is an 8-bit encoding. Each bit represents a vertex. `index[i]` is the ith bit
 * If the value at the ith vertex is < isovalue, `index[i]` = 1. Else, `index[i]` = 0
 */
static auto calculate_cube_index(GridCell const& cell, float const iso_value)
{
    unsigned int cubeIndex{0};
    for (unsigned int i{0}; i < 8; i++)
    {
        if (cell.sdf_values[i] < iso_value)
        {
            cubeIndex |= (1 << i);
        }
    }

    return cubeIndex;
}

auto interpolate(glm::vec3 const& lhs_pos, float lhs_sdf, glm::vec3 const& rhs_pos, float rhs_sdf, float iso_value)
{
    auto epsilon{std::numeric_limits<float>::epsilon()};
    if (std::fabs(iso_value - lhs_sdf) < epsilon)
    {
        return lhs_pos;
    }
    if (std::fabs(iso_value - rhs_sdf) < epsilon)
    {
        return rhs_pos;
    }
    if (std::fabs(lhs_sdf - rhs_sdf) < epsilon)
    {
        return lhs_pos;
    }

    return ((iso_value - lhs_sdf) / (rhs_sdf - lhs_sdf)) * (rhs_pos - lhs_pos) + lhs_pos;
}

auto get_intersection_coordinates(GridCell const& cell, int intersections_key, float isovalue)
{
    std::array<glm::vec3, 12> intersections{};

    unsigned int idx{0};
    while (intersections_key)
    {
        if (intersections_key & 1)
        {
            auto const& [v1, v2]        = edgeToVertices[idx];
            glm::vec3 intersectionPoint = interpolate(cell.vertices[v1], cell.sdf_values[v1], cell.vertices[v2], cell.sdf_values[v2], isovalue);
            intersections[idx]          = intersectionPoint;
        }
        idx++;
        intersections_key >>= 1;
    }

    return intersections;
}

auto get_triangles(std::array<glm::vec3, 12> const& intersections, unsigned int cubeIndex)
{
    std::vector<Triangle> triangles{};

    for (auto const& optional_triangle_indices : triangleTable[cubeIndex])
    {
        if (optional_triangle_indices.has_value())
        {
            auto const& triangle_indices = optional_triangle_indices.value();
            triangles.push_back(Triangle{intersections[triangle_indices.x], intersections[triangle_indices.y], intersections[triangle_indices.z]});
        }
    }

    return triangles;
}

static auto triangulate_cell(GridCell const& cell, float iso_value)
{
    auto const cube_index{calculate_cube_index(cell, iso_value)};
    int const  intersections_key{edgeTable[cube_index]};
    auto const intersection_coordinates{get_intersection_coordinates(cell, intersections_key, iso_value)};
    return get_triangles(intersection_coordinates, cube_index);
}

static auto hash_vertices_to_indices(std::vector<Triangle> const& triangles)
{
    std::unordered_map<glm::vec3, unsigned int> vertices_map{};
    std::vector<glm::vec3>                      vertices{};
    std::vector<glm::uvec3>                     indices{};

    for (auto const& triangle : triangles)
    {
        // Add vertices to the vertices vector if not already present
        for (auto const& vertex : triangle)
        {
            if (!vertices_map.contains(vertex))
            {
                vertices.push_back(vertex);
                vertices_map[vertex] = static_cast<unsigned int>(vertices.size()) - 1;
            }
        }
        indices.emplace_back(vertices_map[triangle[0]], vertices_map[triangle[1]], vertices_map[triangle[2]]);
    }

    return std::make_pair(vertices, indices);
}

auto mesh_from_sdf_sampling(std::vector<float> const& sdf_sampling, float const boxSize, float const meshing_step_size, unsigned int const sampling_count) -> Mesh
{
    std::vector<Triangle> triangles{};

    float isovalue{0};

    unsigned int sampling_count_squared{sampling_count * sampling_count};

    for (unsigned int i{0}; i < sampling_count - 1; i++)
    {
        for (unsigned int j{0}; j < sampling_count - 1; j++)
        {
            for (unsigned int k{0}; k < sampling_count - 1; k++)
            {
                float x{static_cast<float>(i) * meshing_step_size - boxSize / 2.0f};
                float y{static_cast<float>(j) * meshing_step_size - boxSize / 2.0f};
                float z{static_cast<float>(k) * meshing_step_size - boxSize / 2.0f};

                GridCell cell{
                    // clang-format off
                    {{
                    {x, y, z}, {x + meshing_step_size, y, z},
                    {x + meshing_step_size, y, z + meshing_step_size}, {x, y, z + meshing_step_size},
                    {x, y + meshing_step_size, z}, {x + meshing_step_size, y + meshing_step_size, z},
                    {x + meshing_step_size, y + meshing_step_size, z + meshing_step_size}, {x, y + meshing_step_size, z + meshing_step_size}
                    }},
                    {
                    sdf_sampling[i + j * sampling_count + k * sampling_count_squared],
                    sdf_sampling[(i + 1) + j * sampling_count + k * sampling_count_squared],
                    sdf_sampling[(i + 1) + j * sampling_count + (k + 1) * sampling_count_squared],
                    sdf_sampling[i + j * sampling_count + (k + 1) * sampling_count_squared],
                    sdf_sampling[i + (j + 1) * sampling_count + k * sampling_count_squared],
                    sdf_sampling[(i + 1) + (j + 1) * sampling_count + k * sampling_count_squared],
                    sdf_sampling[(i + 1) + (j + 1) * sampling_count + (k + 1) * sampling_count_squared],
                    sdf_sampling[i + (j + 1) * sampling_count + (k + 1) * sampling_count_squared]
                    }
                    // clang-format on
                };

                auto cellTriangles{triangulate_cell(cell, isovalue)};
                for (const auto& cellTriangle : cellTriangles)
                    triangles.push_back(cellTriangle);
            }
        }
    }

    auto const [vertices, indices] = hash_vertices_to_indices(triangles);

    // std::vector<glm::vec3>  vertices{};
    // std::vector<glm::uvec3> indices{};

    // for (size_t i{0}; i < triangles.size(); i++)
    // {
    //     vertices.push_back(triangles[i][0]);
    //     vertices.push_back(triangles[i][1]);
    //     vertices.push_back(triangles[i][2]);

    //     indices.emplace_back(3 * i, 3 * i + 1, 3 * i + 2);
    // }

    return Mesh{vertices, indices};
}

} // namespace Meshing