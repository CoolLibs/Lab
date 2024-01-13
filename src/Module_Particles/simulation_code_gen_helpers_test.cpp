#if LAB_ENABLE_TESTS
#include "simulation_code_gen_helpers.h"
#include "doctest/doctest.h"

TEST_CASE("buffer_to_vec")
{
    CHECK(Lab::buffer_to_vec(2, "_positions", "gid") == "vec2(_positions[gid * 2 + 0], _positions[gid * 2 + 1])");
    CHECK(Lab::buffer_to_vec(3, "_positions", "gid") == "vec3(_positions[gid * 3 + 0], _positions[gid * 3 + 1], _positions[gid * 3 + 2])");
}

TEST_CASE("vec_to_buffer")
{
    CHECK(Lab::vec_to_buffer(2, "_velocities", "gid", "particle.velocity") ==
          R"(_velocities[gid * 2 + 0] = particle.velocity[0];
_velocities[gid * 2 + 1] = particle.velocity[1];
)");
    CHECK(Lab::vec_to_buffer(3, "_positions", "gid", "particle.position") ==
          R"(_positions[gid * 3 + 0] = particle.position[0];
_positions[gid * 3 + 1] = particle.position[1];
_positions[gid * 3 + 2] = particle.position[2];
)");
}

#endif