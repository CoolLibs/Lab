#include "microcode_generator.h"
#if LAB_ENABLE_TESTS

#include "doctest/doctest.h"

TEST_CASE("buffer_to_vec")
{
    CHECK(Lab::buffer_to_vec(2, "_velocities", "gid") == "vec2(_velocities[gid * 2 + 0], _velocities[gid * 2 + 1])");
    CHECK(Lab::buffer_to_vec(3, "_positions", "gid") == "vec3(_positions[gid * 3 + 0], _positions[gid * 3 + 1], _positions[gid * 3 + 2])");
}

TEST_CASE("vec_to_buffer")
{
    CHECK(Lab::vec_to_buffer(2, "_velocities", "gid", "particle.velocity") ==
          R"(_velocities[gid * 2 + 0] = particle.velocity.x;
_velocities[gid * 2 + 1] = particle.velocity.y;
)");
    CHECK(Lab::vec_to_buffer(3, "_positions", "gid", "particle.position") ==
          R"(_positions[gid * 3 + 0] = particle.position.x;
_positions[gid * 3 + 1] = particle.position.y;
_positions[gid * 3 + 2] = particle.position.z;
)");
}

TEST_CASE("vec_zero")
{
    CHECK(Lab::vec_zero(2) == "vec2(0.)");
    CHECK(Lab::vec_zero(3) == "vec3(0.)");
    CHECK(Lab::vec_zero(4) == "vec4(0.)");
}

#endif
