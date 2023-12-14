#pragma once

namespace Lab {

auto constexpr version = R"glsl()glsl";

auto constexpr uniforms = R"glsl(
uniform float _particle_size;
)glsl";

auto constexpr includes = R"glsl()glsl";

auto constexpr buffers = R"glsl(
layout(std430, binding = 0) buffer _positions_buffer
{
    float _positions[];
};

layout(std430, binding = 1) buffer _velocities_buffer
{
    float _velocities[];
};

layout(std430, binding = 2) buffer _sizes_buffer
{
    float _sizes[];
};

layout(std430, binding = 3) buffer _lifetimes_buffer
{
    float _lifetimes[];
};
)glsl";

} // namespace Lab